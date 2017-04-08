// This piece of code receives a test file and a cache with a bunch of
// parent sets. Then, for each parent set it classifies all entries from the
// test file.
//
// The output of this script follows the same format as the cache, but for
// every parent set a MAP is printed as well.


#include "../../../base/dataset.h"
#include "../../../util/file.h"
#include "../../../util/error_output.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;


int main(int argc, char* argv[])
{
  if(argc != 4)
  {
    cerr << "Usage:\n    ./" << argv[0]
         << " [test filename] [label cache filename] [output path]\n";
    return 1;
  }

  string test_filename = argv[1], label_cache_filename = argv[2];
  string output_path = argv[3];
  
  // Making sure the output path exists...
  // And create it if not.
  if(!util::File::CreateDirectory(output_path))
  {
    util::Error::Print("Could not find nor create the output path.");
    return 1;
  }
  
  
  
  return 0; 
}
