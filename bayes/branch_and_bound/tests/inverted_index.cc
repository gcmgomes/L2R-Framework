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
#include "../inverted_index.h"
#include "../../../base/dataset.h"
#include "../../../util/discretizer.h"

using namespace std;

int main() {
  string file_path =
      "/Users/gcmgomes/git_repositories/learning_to_rank_parse/bayes/x.txt";
  util::Discretizer disc(util::Discretizer::Mode::TREE_BASED_UNSUPERVISED, 2);
  vector<bayes::branch_and_bound::Instance> instances;
  bayes::branch_and_bound::Instance::ParseDataset(file_path, disc, instances);
  bayes::branch_and_bound::InvertedIndex index(instances);
  instances.clear();
  cout << index.ToString() << endl;
  set<unsigned> parent;
  std::vector<unsigned> config;
  parent.insert(1);
  config.push_back(0);
  unordered_set<unsigned> intersect;
  index.ConfigurationIntersect(parent, config, intersect);
  auto it = intersect.begin();
  while(it != intersect.end()) {
    cout << *it << endl;
    ++it;
  }
  cout << "Base Intersect: " << index.BaseVariableIntersect(2, 1, intersect) << endl;
  cout << "Total documents: " << index.TotalPoints() << endl;
  cout << "Variable 0 valued at 1 has: " << index.Count(0, 1) << " points" << endl;
  return 0;
}
