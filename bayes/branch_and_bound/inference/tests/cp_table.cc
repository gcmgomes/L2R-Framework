#include <cassert>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <cmath>
#include <unordered_set>
#include "../cp_table.h"
#include "../../variable.h"

using namespace std;

int main(int argc, char** argv) {
  string file_path = "/var/tmp/data.txt";
  if (argc > 1) {
    file_path = argv[1];
  }
  util::Discretizer disc(util::Discretizer::Mode::TREE_BASED_UNSUPERVISED, 2);
  vector<bayes::branch_and_bound::Instance> instances;
  bayes::branch_and_bound::Instance::ParseDataset(file_path, disc, instances);
  vector<bayes::branch_and_bound::Cache> caches;
  bayes::branch_and_bound::InvertedIndex index(instances);
  instances.clear();
  std::vector<bayes::branch_and_bound::inference::CPTable> cp_tables;
  unsigned i = 0;
  while(i < index.index().size()) {
    bayes::branch_and_bound::Bitset parent_set(index.index().size());
    parent_set.Set((i+1) % index.index().size(), true);
    cp_tables.emplace_back(i, parent_set);
    cp_tables.back().Build(index);
    cout << i << endl;
    cout << cp_tables.back().ToString() << endl; 
    i++;
  }
  return 0;
}

