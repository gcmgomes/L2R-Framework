#ifndef _error_output_h
#define _error_output_h

// This class standardizes the way to output errors.
// TODO: Think in changing the name from error_output to std_output
//       And maybe create another classes, like a "warning" class,
//       Or a "Log" class.


#include <string>

namespace util {
  class Error {
    public:
      static void Print(const std::string& msg);
      static void Print(const char* msg);
  };
} // namespace util


#endif
