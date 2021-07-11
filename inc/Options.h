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
     Options_Parser()
          : m_desc("Options")
     {
          m_desc.add_options()
          ("help,h",   "Help")
          ("include,i",  po::value<std::vector<fs::path>>(), "Directories for scan")
          ("exclude,e",po::value<std::vector<fs::path>>(), "Excluded inner directories")
          ("depth,d",  po::value<std::size_t>(), "Scan depth - default 0")
          ("size,s",   po::value<std::size_t>(), "Minimum file's size - default 1 byte")
          ("masks,m",  po::value<std::vector<std::string>>(), "File masks")
          ("block,b",  po::value<std::size_t>(), "Block size")
          ("algo,a",   po::value<std::string>(), "Hash algorithm - default crc32");    
     }
     
     boost::optional<Options> Parse(int argc, char ** argv)
     {
          try {
               po::store(parse_command_line(argc, argv, m_desc), m_vm);
               po::notify(m_vm);

               if (m_vm.count("help")) {
                    std::cout << m_desc << '\n';
                    return boost::optional<Options>();
               }

               Options ops;

               if (m_vm.count("include")) {
                    ops.incPaths = m_vm["include"].as<std::vector<fs::path>>();
                    for(auto f : ops.incPaths)
                         std::cout << "input: " << f << std::endl;
               }
               if (m_vm.count("exclude")) {
                    ops.excPaths = m_vm["exclude"].as<std::vector<fs::path>>();
                    for(auto f : ops.excPaths)
                         std::cout << "exclude: " << f << std::endl;
               }
               if (m_vm.count("depth")) {
                    ops.depth = m_vm["depth"].as<std::size_t>();
                    std::cout << "depth: " << ops.depth << std::endl;
               }
               if (m_vm.count("size")) {
                    ops.minSize = m_vm["size"].as<std::size_t>();
                    std::cout << "size: " << ops.minSize << std::endl;
               }
               if (m_vm.count("masks")) {
                    ops.masks = m_vm["masks"].as<std::vector<std::string>>();
                    for(auto f : ops.masks)
                         std::cout << "masks: " << f << std::endl;
               }
               if (m_vm.count("block")) {
                    ops.block = m_vm["block"].as<std::size_t>();
                    std::cout << "block: " << ops.block << std::endl;
               }
               if (m_vm.count("algo")) {
                    ops.algo = m_vm["algo"].as<std::string>();
                    std::cout << "algo: " << ops.algo << std::endl;
               }
               return ops;
          }
          catch(const std::exception& e)
          {
               std::cerr << e.what() << '\n';
               return boost::optional<Options>();
          }             
     }
private:
     po::options_description m_desc;
     po::variables_map m_vm;
};
#endif