#include "inc/Options.h"
#include "iostream"
#include "boost/program_options.hpp"

namespace po =  boost::program_options;

/// @file
/// @brief Main function. Find dublicate files.
/// @author btv<example@example.com>
Options options_parser(int argc, const char * argv[])
{
   Options ops;
   try
   {
      po::options_description desc{"Options"};
      desc.add_options()
         ("help,h",   "Help")
         ("input,i",  po::value<std::vector<path>>(), "Directories for scan")
         ("exclude,e",po::value<std::vector<path>>(), "Excluded inner directories")
         ("depth,d",  po::value<std::size_t>(), "Scan depth")
         ("size,s",   po::value<std::size_t>(), "Minimum file's size")
         ("masks,m",   po::value<std::vector<std::string>>(), "File masks")
         ("block,b",  po::value<std::size_t>(), "Block size")
         ("algo,a",    po::value<std::string>(), "Hash algorithm");

      po::variables_map vm;
      po::store(parse_command_line(argc, argv, desc), vm);
      po::notify(vm);

      if (vm.count("help"))
         std::cout << desc << '\n';

      else if (vm.count("input")) {
         ops.scanDirs = vm["input"].as<std::vector<path>>();
         for(auto f : ops.scanDirs)
            std::cout << "input: " << f << std::endl;
      }
      if (vm.count("exclude")) {
         ops.excDirs = vm["exclude"].as<std::vector<path>>();
         for(auto f : ops.excDirs)
            std::cout << "exclude: " << f << std::endl;
      }
      if (vm.count("depth")) {
         ops.depthScan = vm["depth"].as<std::size_t>();
         std::cout << "depth: " << ops.depthScan << std::endl;
      }
      if (vm.count("size")) {
         ops.fileSize = vm["size"].as<std::size_t>();
         std::cout << "size: " << ops.fileSize << std::endl;
      }
      if (vm.count("masks")) {
         ops.fileMasks = vm["masks"].as<std::vector<std::string>>();
         for(auto f : ops.fileMasks)
            std::cout << "masks: " << f << std::endl;
      }
      if (vm.count("block")) {
         ops.blockSize = vm["block"].as<std::size_t>();
         std::cout << "block: " << ops.blockSize << std::endl;
      }
      if (vm.count("algo")) {
         ops.hashAlgo = vm["algo"].as<std::string>();
         std::cout << "algo: " << ops.hashAlgo << std::endl;
      }
   }
   catch(const std::exception& e)
   {
      std::cerr << e.what() << '\n';
   }
   return ops;   
}
int main(int argc, const char * argv[]) {
   auto options = options_parser(argc,argv);
   
   return 0;
}  
