#include "../instance.h"
#include "../../../base/dataset.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

int main(int argc, char** argv) {
  if (argc < 3) {
    cerr << argv[0] << " [input file] [bin count]" << endl;
    return 0;
  }
  string input_file_path = argv[1];
  unsigned bin_count = 0;
  sscanf(argv[2], "%u", &bin_count);
  util::Discretizer disc(util::Discretizer::Mode::TREE_BASED_UNSUPERVISED,
                         bin_count);
  vector<bayes::branch_and_bound::Instance> instances;
  bayes::branch_and_bound::Instance::ParseDataset(input_file_path, disc,
                                                  instances);
  unsigned id = 1;
  cout << "L"; 
  while(id < instances[0].values().size()) {
    cout << " " << id;
    id++;
  }
  cout << endl;
  id = 0;
  while (id < instances.size()) {
    cout << instances[id].ToString() << endl;
    id++;
  }
  return 0;
}
