#include <boost/uuid/uuid.hpp>
#include <boost/regex.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/crc.hpp>

#include "../inc/Options.h"
#include "../inc/Filter.h"
#include "../inc/FileScanner.h"

PathFinder::PathFinder(const std::vector<fs::path>& a_excPaths, boost::optional<std::size_t>& a_depth, std::vector<std::string> a_masks, boost::optional<std::size_t>& a_minSize)
{
    m_dirFilter = CreateFilter(a_depth, a_excPaths);
    m_fileFilter =  CreateFilter(a_minSize, a_masks);
}

std::unordered_map<std::size_t,std::set<fs::path>> PathFinder::FindPaths(const Paths& a_paths) {
        std::unordered_map<std::size_t,std::set<fs::path>> result;

        for (auto &path : a_paths) {       
            fs::recursive_directory_iterator it(path), end;
            while (it != end)
            {
                std::pair<fs::path,std::size_t> dirPath = std::make_pair(it->path(), it.level());             
                if (fs::is_directory(it->path()) ) {
                    if (!m_dirFilter->IsValid(dirPath))  it.no_push();            
                }
                else {
                    if (fs::is_regular_file(it->path()) ) {
                        std::pair<fs::path,std::size_t> filePath = std::make_pair(it->path(), fs::file_size(it->path()));
             
                        if (m_fileFilter->IsValid(filePath) ) {
                            auto& uniquepaths = result[filePath.second];
                            uniquepaths.insert(filePath.first);
                        }
                    }
                }
                boost::system::error_code ec;
                it.increment(ec);
                 if (ec) {
                     std::cerr << "Error while accessing: " << it->path().string() << " :: " << ec.message() << '\n';
              }
            }
        }
        if (!result.empty()) {
            auto it = result.begin();
            while(it != result.end()) {
                if (it->second.size() < 2) {
                  it = result.erase(it);
                }
                else ++it;
            }
        }
        return result;
}

std::unique_ptr<Filter> PathFinder::CreateFilter(boost::optional<std::size_t>& a_depth, const Paths& a_paths)
{   
        std::size_t depth = 0;
        if (a_depth) {
            depth = a_depth.get();
        }
        auto depthFilter = std::make_unique<DepthFilter>(depth);
      
        auto excFilter = std::make_shared<ExcludeFilter>(a_paths);
        depthFilter->SetNext(excFilter);
      
        return depthFilter;
}

std::unique_ptr<Filter> PathFinder::CreateFilter(boost::optional<std::size_t>& a_minSize, const std::vector<std::string>& a_masks) 
{
        std::size_t minSize = 1;
        if (a_minSize) {
            minSize = a_minSize.get();
        }
        auto minSizeFilter = std::make_unique<MinSizeFilter>(minSize);
      
        auto masksFilter = std::make_shared<MasksFilter>(a_masks);
        minSizeFilter->SetNext(masksFilter);
      
        return minSizeFilter; 
}
FileScanner::FileScanner(boost::optional<std::size_t>& a_block, boost::optional<std::string>& a_algo) {
	  m_block =  1024;
    if (a_block) {
	    m_block = a_block.get();
    }
    m_hashFunc = CreateHash(a_algo);
}

std::list<std::vector<fs::path>> FileScanner::Scan(std::unordered_map<std::size_t,std::set<fs::path>> a_paths) {
	    std::list<std::vector<fs::path>> result;
      for (const auto&  group : a_paths) {
          auto duplicates = CheckPaths(group.second);
          auto groups = Groups(duplicates);
          std::copy(groups.begin(),groups.end(),std::back_inserter(result));
      }
      return result;
}

std::unordered_map<std::string,std::pair<std::fstream, std::size_t>> FileScanner::CheckPaths(const std::set<fs::path>& a_paths) {
    std::unordered_map<std::string,std::pair<std::fstream, std::size_t>> result;
    
    for (const auto& path : a_paths) {      
        auto str_path = path.string();       
        std::fstream read_stream(str_path,std::fstream::in);
        result[str_path] = std::make_pair(std::move(read_stream),0);          
    }
    std::vector<char> buffer(m_block);
    bool endFiles = false;
  
    while (!result.empty() && !endFiles) {
      for (auto& elem : result) {
        memset(buffer.data(), 0, m_block);
        auto& value = elem.second;
        auto readBytes = value.first.readsome(buffer.data(), m_block);
        endFiles = static_cast<size_t>(readBytes) < m_block;
        value.second = m_hashFunc(buffer.data(), buffer.size());
      }
      
      auto it = result.begin();
      while (it != result.end()) {
        size_t count = 0;
        size_t curHash = it->second.second;
        std::for_each(result.begin(), result.end(), 
          [curHash, &count](const auto& value)
          {
            if (value.second.second == curHash) {
              ++count;
            }
          }
        );
     
        if (count == 1) {
          it = result.erase(it);
        }
        else {
          ++it;
        }
      }
    }

    return result;
}

std::list<std::vector<fs::path>> FileScanner::Groups(std::unordered_map<std::string,std::pair<std::fstream, std::size_t>>& a_paths)
{
  std::list<std::vector<fs::path>> result;

  while (!a_paths.empty()) {
    std::vector<fs::path> paths;
    
    std::size_t curHash = a_paths.begin()->second.second;
    auto it = a_paths.begin();
    while (it != a_paths.end()) {
      if (it->second.second == curHash) {
        paths.emplace_back(it->first);
        it = a_paths.erase(it);
      }
      else {
        ++it;
      }
    }
    if (!paths.empty()) {
      result.emplace_back(paths);
    }
  }

  return result;
}

boost::function<std::size_t (char*, std::size_t)> FileScanner::CreateHash(boost::optional<std::string>& a_algo) {
  if (a_algo) {
    if (a_algo.get() == "crc16") {
      return HashFunc<boost::crc_16_type>();
    }
    else if (a_algo.get() == "crc32") {
      return HashFunc<boost::crc_32_type>();
    }
  }
  return HashFunc<boost::crc_32_type>();
}

template<typename T>
boost::function<std::size_t (char* a_buffer, std::size_t a_count)> FileScanner::HashFunc() {
    return [](char* a_buffer, std::size_t a_count) {
      T algo;
      algo.process_bytes(a_buffer, a_count);
      return algo.checksum();
    };
}