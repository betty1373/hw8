#include "inc/Logger.h"

/// @file
/// @brief Main function. Test for processing commands.
/// @author btv<example@example.com>
int main(int argc, char * argv[]) {
   if (argc < 2) {
      std::cerr<<"Use"<<argv[0]<<"<bulk_size>\n";
      return EXIT_FAILURE;
   }
   size_t num = std::atoi(argv[1]);
   if (num<=0) {
       std::cerr<<"Bulk size must be greater than 0\n";
      return EXIT_FAILURE;
   }
   auto cmdReader = CmdReader::Create(num);
   auto consoleLogger = ConsoleLogger::Create(cmdReader);
   auto fileLogger = FileLogger::Create(cmdReader);
   try {
      cmdReader->Work();
   }
   catch (std::runtime_error& e) {
      std::cerr <<"Error :"<< e.what()<<std::endl;
      return EXIT_FAILURE;
   }
   return 0;
}  
