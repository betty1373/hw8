#ifndef Filter_H
#define Filter_H
#include <set>
#include <unordered_map>
#include <string>
#include <vector>
#include <list>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <memory>

using Path = std::pair<boost::filesystem::path, std::size_t>; 
using Paths = std::vector<boost::filesystem::path>;

class Mask
{
public:
  explicit Mask(const std::string& a_mask)
  {
    auto mask = a_mask;
    boost::replace_all(mask, ".", "\\.");
    boost::replace_all(mask, "*", "\\*");
    boost::replace_all(mask, "?", "\\?");
    boost::replace_all(mask, "\\?", ".");
    boost::replace_all(mask, "\\*", ".*");
    m_regex = boost::regex(mask, boost::regex::icase );
  }

  bool Valid(const std::string& a_strValue)
  {
    return boost::regex_match(a_strValue, m_what, m_regex);
  }
private:
  boost::regex m_regex;
  boost::smatch m_what;
};

class IFilter
{
public:
  virtual std::shared_ptr<IFilter>& SetNext(const std::shared_ptr<IFilter>& a_Filter) = 0;
  virtual bool IsValid(const Path& a_path) = 0;
};

class Filter : public IFilter
{
public:
  Filter()
  { }

  std::shared_ptr<IFilter>& SetNext(const std::shared_ptr<IFilter>& a_Filter) override
  {
    this->nextFilter = a_Filter;
    return nextFilter;
  }

  bool IsValid(const Path& a_path) override
  {
    if ( nextFilter ) {
      return nextFilter->IsValid(a_path);
    }

    return true;
  }

private:
  std::shared_ptr<IFilter> nextFilter;
};

class DepthFilter : public Filter
{
public:
  DepthFilter(std::size_t a_depth)
    : Filter()
    , m_depth(a_depth)
  { }

  bool IsValid(const Path& a_path) override
  {
    if (a_path.second >= m_depth) {
      return false;    
    }
    return Filter::IsValid(a_path);
  }

private:
  std::size_t m_depth;
};

class ExcludeFilter : public Filter
{
public:
  ExcludeFilter(const Paths& a_paths)
    : Filter()
    , m_excPaths(a_paths)
  { }

  bool IsValid(const Path& a_path) override
  {
    if (!m_excPaths.empty()) {
      auto path = a_path.first;
      auto it = std::find_if(m_excPaths.begin(), m_excPaths.end(), 
        [path](const boost::filesystem::path& exc_path) {
          return path == exc_path;
        } 
      );
    
      if (it != m_excPaths.end()) {
        return false;    
      }
    }
    
    return Filter::IsValid(a_path);
  }

private:
  Paths m_excPaths;
};

class MinSizeFilter : public Filter
{
public:
  MinSizeFilter(std::size_t a_size)
    : Filter()
    , m_size(a_size)
  { }

  bool IsValid(const Path& a_path) override
  {
    if (boost::filesystem::file_size(a_path.first) < m_size) {;
      return false;    
    }
    return Filter::IsValid(a_path);
  }

private:
  std::size_t m_size;
};

class MasksFilter : public Filter
{
public:
  MasksFilter(const std::vector<std::string>& a_masks)
    : Filter()
  { 
    for (auto& mask : a_masks) {
      m_masks.emplace_back(Mask(mask));
    }
  }

  bool IsValid(const Path& a_path) override
  {
    if (!m_masks.empty()) {
      auto filename = a_path.first.filename().string();
      return std::any_of(m_masks.begin(), m_masks.end(), 
        [filename](Mask& mask) {
          return mask.Valid(filename);
        } 
      );
    }
    return Filter::IsValid(a_path);
  }

private:
  std::vector<Mask> m_masks;
};
#endif
