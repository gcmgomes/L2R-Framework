#ifndef _util_file_h
#define _util_file_h

// This is a set of utilitary classes to manipulate the filesystem.
//  For now it's just working on POSIX systems.


#include <string>


namespace util {
  class File {
    public:
      static bool CreateDirectory(const std::string& path);
      static bool CreateDirectory(const char* path);
  };
} // namespace util


#endif
