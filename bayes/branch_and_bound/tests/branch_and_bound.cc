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

int main(int argc, char** argv) {
  string file_path =
      "/var/tmp/data.txt";
  if (argc > 1) {
    file_path = argv[1];
  }
  util::Discretizer disc(util::Discretizer::Mode::TREE_BASED_UNSUPERVISED, 2);
  vector<bayes::branch_and_bound::Instance> instances;
  bayes::branch_and_bound::Instance::ParseDataset(file_path, disc, instances);
  bayes::branch_and_bound::InvertedIndex index(instances);
  instances.clear();
  vector<bayes::branch_and_bound::Cache> caches;
  bayes::branch_and_bound::Cache::LoadCaches("/var/tmp", index.index().size(), 1,
                                             caches);
  vector<bayes::branch_and_bound::ExternalQueue> external_queues;
  vector<bayes::branch_and_bound::Variable> variables;
  bayes::branch_and_bound::Variable::InitializeVariables(
      index, variables, external_queues, caches);
  bayes::branch_and_bound::BranchAndBound bnb(variables);

  bayes::branch_and_bound::Graph best = bnb.Run(1.0);
  std::cerr << "Best graph is:" << std::endl << best.ToString() << std::endl;
  return 0;
}
