#include "file.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include <stack>

namespace util {
bool File::CreateDirectory(const std::string& path)
{
  return File::CreateDirectory(path.c_str());
}

bool File::CreateDirectory(const char* path)
{
  // This piece of code tries to create a folder recursively.
  // If mkdir returns an error, then we try to create the parent
  // folder first; this logic applies recursively until the root

  int status, index=0;
  std::string current(path);
  std::stack<std::string> all_folders;

  if(current.empty()) return false;

  if(*current.rbegin() != '/')
    current += '/';

  while(!current.empty())
  {
    while(!current.empty() && *current.rbegin() != '/')
      current.pop_back();
    status = mkdir(current.c_str(),
                   S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    // If the folder already exists, then just let it there...
    if(index==0 && errno==EEXIST) return true;
    if(status==0) break;
    else all_folders.push(current);
    current.pop_back();
    index+=1;
  }
  
  while(!all_folders.empty())
  {
    status = mkdir(all_folders.top().c_str(),
                   S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if(status != 0) return false;
    all_folders.pop();
  }
  
  return true;
}

bool File::DirectoryExists(const char* path)
{
  struct stat sb;
  return (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode));
}

bool File::DirectoryExists(const std::string& path) {
  return File::DirectoryExists(path.c_str()); 
}

bool File::FileExists(const char* path)
{
  struct stat sb;
  return (stat(path, &sb) == 0 && S_ISREG(sb.st_mode));
}

bool File::FileExists(const std::string& path) {
  return File::FileExists(path.c_str()); 
}


} // namespace util
