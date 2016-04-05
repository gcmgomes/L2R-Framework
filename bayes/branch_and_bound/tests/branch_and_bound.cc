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
#include "../branch_and_bound.h"

using namespace std;

int main() {
  string file_path =
      "/Users/gcmgomes/git_repositories/learning_to_rank_parse/bayes/x.txt";
  util::Discretizer disc(util::Discretizer::Mode::TREE_BASED_UNSUPERVISED, 2);
  vector<bayes::branch_and_bound::Instance> instances;
  bayes::branch_and_bound::Instance::ParseDataset(file_path, disc, instances);
  bayes::branch_and_bound::BranchAndBound bnb(
      instances,
      bayes::branch_and_bound::Criterion::AKAIKE_INFORMATION_CRITERION);

  bayes::branch_and_bound::Graph best = bnb.Run(1.0);
  std::cerr << "Best graph is:" << std::endl << best.ToString() << std::endl;
  return 0;
}
