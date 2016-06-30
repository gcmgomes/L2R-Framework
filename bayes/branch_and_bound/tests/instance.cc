#include "../instance.h"
#include "../../../base/dataset.h"
#include "../../../util/discretizer.h"
#include <algorithm>
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
  if (argc < 2) {
    cerr << "[input file]" << endl;
    return 0;
  }
  string file_path = argv[1];
  vector<bayes::branch_and_bound::Instance> instances;
  util::Discretizer disc(util::Discretizer::Mode::TREE_BASED_UNSUPERVISED, 2);
  bayes::branch_and_bound::Instance::ParseDataset(file_path, disc, instances);
  auto it = instances.cbegin();
  while (it != instances.cend()) {
    cout << it->ToString() << endl;
    ++it;
  }
  return 0;
}
