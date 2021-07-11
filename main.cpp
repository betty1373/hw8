#include "inc/FileScanner.h"
#include "iostream"

/// @file
/// @brief Main function. Find dublicate files.
/// @author btv<example@example.com>
int main(int argc, char** argv) {
   Options_Parser options_parser;
   auto options = options_parser.Parse(argc, argv);
   if (!options)
      return 0;
 
  PathFinder pathFinder(options.get().excPaths, options.get().depth, options.get().masks, options.get().minSize);
  auto groupPath = pathFinder.FindPaths(options.get().incPaths);

  FileScanner fileScanner(options.get().block, options.get().algo);
  
  auto duplicates = fileScanner.Scan(groupPath);

  for (auto& dup : duplicates) {
    for (auto& path : dup) {
      std::cout << path << std::endl;
    }
    std::cout << std::endl;
  }
   return 0;
}  
