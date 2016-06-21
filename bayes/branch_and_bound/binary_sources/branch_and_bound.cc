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
#include "../branch_and_bound.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc < 4) {
    cerr << argv[0] << " [input file] [cache directory] [bin count]" << endl;
    return 0;
  }
  string input_file_path = argv[1], cache_directory = argv[2];
  unsigned bin_count = 0;
  sscanf(argv[3], "%u", &bin_count);
  util::Discretizer disc(util::Discretizer::Mode::TREE_BASED_UNSUPERVISED, bin_count);
  vector<bayes::branch_and_bound::Instance> instances;
  bayes::branch_and_bound::Instance::ParseDataset(input_file_path, disc, instances);
  bayes::branch_and_bound::InvertedIndex index(instances);
  instances.clear();
  vector<bayes::branch_and_bound::Cache> caches;
  bayes::branch_and_bound::Cache::LoadCaches(cache_directory,
                                             index.index().size(), 1, caches);

  vector<bayes::branch_and_bound::inference::CPTable> cp_tables;
  vector<bayes::branch_and_bound::Variable> variables;
  bayes::branch_and_bound::Variable::InitializeVariables(
      index, variables, caches, cp_tables);
  index.clear();
  bayes::branch_and_bound::BranchAndBound bnb(variables);

  bayes::branch_and_bound::Graph best = bnb.Run(1.0);
  std::cout << "Best graph is:" << std::endl << best.ToString() << std::endl;
  return 0;
}
