#ifndef Options_H
#define Options_H
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <boost/program_options.hpp>
#include <boost/optional/optional_io.hpp>
#include <iostream>
/// @file
/// @brief Class of program options 
/// @author btv<example@example.com>
namespace fs = boost::filesystem;
namespace po =  boost::program_options;

struct Options {
     std::vector<fs::path> incPaths;
     std::vector<fs::path> excPaths;
     boost::optional<std::size_t> depth;
     boost::optional<std::size_t> minSize;
     std::vector<std::string> masks;
     boost::optional<std::size_t> block; 
     boost::optional<std::string> algo;
};

class Options_Parser{
 public: 
     Options_Parser();
     boost::optional<Options> Parse(int argc, char ** argv);
     
private:
     po::options_description m_desc;
     po::variables_map m_vm;
};
#endif