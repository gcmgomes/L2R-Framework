#include <iostream>
#include <fstream>
#include "k2.h"
#include "../base/dataset.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc < 4) {
    cerr << argv[0] << " [dataset] [feature count] [max parents]" << endl;
    return 0;
  }
  unsigned f_count = 0, parent_count = 0;
  sscanf(argv[2], "%u", &f_count);
  sscanf(argv[3], "%u", &parent_count);
  base::Dataset* d = new ::base::Dataset();

  d->Parse(argv[1]);

  ::bayes::K2 k2(d, 5);
  k2.Init(100000);
  unsigned i = 0;
  std::vector<unsigned> v;
  while(i < f_count) {
    v.push_back(i+1);
    i++;
  }
  k2.BuildNetwork(parent_count, v);
  k2.PrintNetwork();
}
