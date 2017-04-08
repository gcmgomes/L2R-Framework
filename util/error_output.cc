#include "error_output.h"

#include <iostream>

namespace util {
  void Error::Print(const std::string& msg)
  {
    return Error::Print(msg.c_str());
  }

  void Error::Print(const char* msg)
  {
    std::cerr << "\e[1;31m";  // ANSI light red code.
    std::cerr << "Error: ";
    std::cerr << "\e[0m"; // ANSI black code.
    
    std::cerr << msg << std::endl;
  }
}
