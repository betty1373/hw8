#ifndef FileScanner_H
#define FileScanner_H
#include <boost/uuid/uuid.hpp>
#include <boost/regex.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/crc.hpp>

#include "Options.h"
#include "Filter.h"
/// @file
/// @brief Class of find suitable files
/// @author btv<example@example.com>

class PathFinder {
public:
/// @brief Construct class scanner
    PathFinder(const std::vector<fs::path>& a_excPaths, boost::optional<std::size_t>& a_depth, 
        std::vector<std::string> a_masks, boost::optional<std::size_t>& a_minSize);
    ~PathFinder() = default;

/// @brief Scan dirs for finding suitable files
    std::unordered_map<std::size_t,std::set<fs::path>> FindPaths(const Paths& a_paths);
private:
/// @brief Filter for dirs
    static std::unique_ptr<Filter> CreateFilter(boost::optional<std::size_t>& a_depth, const Paths& a_paths);
 /// @brief Filter for files
    static std::unique_ptr<Filter> CreateFilter(boost::optional<std::size_t>& a_minSize, const std::vector<std::string>& a_masks);

    std::unique_ptr<Filter> m_dirFilter;
    std::unique_ptr<Filter> m_fileFilter;
};

using HashFunc_t = boost::function<std::size_t (char*, std::size_t)>;
/// @brief File scanner
class FileScanner {
public:
  FileScanner(boost::optional<std::size_t>& a_block, boost::optional<std::string>& a_algo);
/// @brief Finding Duplicates
  std::list<std::vector<fs::path>> Scan(std::unordered_map<std::size_t,std::set<fs::path>> a_paths); 
private:
/// @brief Compare hashes of files 
  std::unordered_map<std::string,std::pair<std::fstream, std::size_t>> CheckPaths(const std::set<fs::path>& a_paths); 
/// @brief Grouping duplicate files
  std::list<std::vector<fs::path>> Groups(std::unordered_map<std::string,std::pair<std::fstream, std::size_t>>& a_paths);

/// @brief Creater for Hash function
  static boost::function<std::size_t (char*, std::size_t)> CreateHash(boost::optional<std::string>& a_algo);
/// @brief Calc Hash 
  template<typename T>
  static boost::function<std::size_t (char* a_buffer, std::size_t a_count)> HashFunc();

private:
  std::size_t m_block;
  boost::function<std::size_t (char*, std::size_t)> m_hashFunc;
};
#endif