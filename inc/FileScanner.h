#ifndef FileScanner_H
#define FileScanner_H
#include <boost/uuid/uuid.hpp>
#include <boost/regex.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/crc.hpp>

#include "Options.h"
/// @file
/// @brief Class of file options
/// @author btv<example@example.com>

class FileOptions {
public:
    fs::path m_path;
    std::vector<std::string> m_hash;
    std::size_t m_size;

    bool operator < (const FileOptions& other) const {
        if (m_size == other.m_size)
            return m_size < other.m_size;

        return m_size < other.m_size;
    }

}
class FileScanner {
public:
/// @brief Find file's dublicates
    // void dublicates(const Options& ops) {
    //     auto m_files = getDirs(ops.get().scanDirs, ops.get().depthScan);
    // };
    
private:
/// @brief Scan dirs
  std::vector<fs::path> getDirs(const boost::optional<std::vector<fs::path>>& inputs, const size_t depth) {
      std::vector<fs::path> result;
      for (auto &d : inputs) {
          
        // if (is_directory(d) && Options::isExclude(d)) 
        //     result.push_back(d);

        boost::filesystem::recursive_directory_iterator it(d), end;
        while (it != end)
        {
            if (it->status().type()==)
            {
                ++it;
                continue;
            }
            else if (it.depth() >= depth)
            {
                ++it;
                continue;
            }
        //    else if (Options::isExclude(dir->path().filename())) 
          //      result.push_back(d);
            ++it;
        }
      }
      return result;
  };
  /// @brief Scan files
  std::vector<fs::path> getFiles(const std::vector<fs::path>& dirs, const std::vector<std::string>& masks, const size_t fileSize) {
      std::vector<fs::path> result;
      for (const auto &d : dirs) {
        boost::filesystem::recursive_directory_iterator it(d), eod;
        while (it != eod)
        {
            if (!is_regular_file(it->path()))
            {
                ++it;
                continue;
            }
            else if (fileSize != 0)
            {
                if (file_size(it->path()) < fileSize) {
                    ++it;
                    continue;
                }
            }
           // else if (Options::isExclude(dir->path().filename())) 
             //   result.push_back(d);
            ++it;
        }
      }
      return result;
  };
};
#endif