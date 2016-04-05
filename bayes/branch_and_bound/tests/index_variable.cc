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
#include "../variable.h"
#include "../../../base/dataset.h"
#include "../../../util/discretizer.h"

using namespace std;

int main() {
  string file_path =
      "/Users/gcmgomes/git_repositories/learning_to_rank_parse/bayes/y.txt";
  util::Discretizer disc(util::Discretizer::Mode::TREE_BASED_UNSUPERVISED, 2);
  vector<bayes::branch_and_bound::Instance> instances;
  bayes::branch_and_bound::Instance::ParseDataset(file_path, disc, instances);
  vector<bayes::branch_and_bound::Cache> caches;
  vector<bayes::branch_and_bound::Variable> variables;
  bayes::branch_and_bound::InvertedIndex index(instances);
  instances.clear();
  bayes::branch_and_bound::Variable::InitializeVariables(index, variables,
                                                         caches);
  auto it = variables.begin();
  ++it;
  while (it != variables.end()) {
    it->BuildCache(index, variables);
    ++it;
  }
  return 0;
}
