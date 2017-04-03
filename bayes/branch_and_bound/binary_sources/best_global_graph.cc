#include "../branch_and_bound.h"
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
#include <vector>

using namespace std;

int main(int argc, char** argv) {
  if (argc < 4) {
    cerr << argv[0] << " [input file] [cache directory] [bin count]" << endl;
    return 0;
  }
  string input_file_path = argv[1], cache_directory = argv[2];
  unsigned bin_count = 0;
  sscanf(argv[3], "%u", &bin_count);
  util::Discretizer disc(util::Discretizer::Mode::TREE_BASED_UNSUPERVISED,
                         bin_count);
  vector<bayes::branch_and_bound::Instance> instances;
  bayes::branch_and_bound::Instance::ParseDataset(input_file_path, disc,
                                                  instances);
  bayes::branch_and_bound::InvertedIndex index(instances);
  instances.clear();
  vector<bayes::branch_and_bound::Cache> caches;
  bayes::branch_and_bound::Cache::LoadCaches(cache_directory,
                                             index.index().size(), caches);
  vector<bayes::branch_and_bound::inference::CPTable> cp_tables;
  vector<bayes::branch_and_bound::Variable> variables;
  bayes::branch_and_bound::Variable::InitializeVariables(index, variables,
                                                         caches, cp_tables);
  bayes::branch_and_bound::Graph graph(variables);
  graph.ReadyForUse(variables);
  auto it = graph.variables().cbegin();
  while (it != graph.variables().cend()) {
    auto high_bits = it->parent_set().high_bits();
    auto jt = high_bits.cbegin();
    while (jt != high_bits.cend()) {
      cout << "add.edge(g, " << *jt + 1 << ", " << it->variable_id() + 1 << ")"
           << endl;
      ++jt;
    }
    ++it;
  }
  return 0;
}
