#ifndef Options_H
#define Options_H
#include <string>
#include <boost/filesystem.hpp>
/// @file
/// @brief Class of program options 
/// @author btv<example@example.com>
using path = boost::filesystem::path;

class Options{
 public: 
     Options() :
        scanDirs ({}),
        excDirs ({}),
        depthScan (0),
        fileSize (1),
        fileMasks ({}),
        blockSize {0},
        hashAlgo ("src32"){};
    
     static bool isExclude(const path& p) {
          auto it = find(excDirs.begin(), excDirs.end(), p);
          return (it == excDirs.end());
     };
        
     std::vector<path> scanDirs;
     std::vector<path> excDirs;
     std::size_t depthScan;
     std::size_t fileSize;
     std::vector<std::string> fileMasks;
     std::size_t blockSize; 
     std::string hashAlgo;
};
#endif