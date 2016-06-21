#include "../ranker.h"
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
  string file_path = "/var/tmp/data.txt";
  if (argc > 1) {
    file_path = argv[1];
  }
  util::Discretizer disc(util::Discretizer::Mode::TREE_BASED_UNSUPERVISED, 2);
  vector<bayes::branch_and_bound::Instance> instances;
  bayes::branch_and_bound::Instance::ParseDataset(file_path, disc, instances);
  vector<bayes::branch_and_bound::Cache> caches;
  bayes::branch_and_bound::InvertedIndex index(instances);
  std::vector<bayes::branch_and_bound::inference::CPTable> cp_tables;
  unsigned i = 0;
  while (i < index.index().size()) {
    bayes::branch_and_bound::Bitset parent_set(index.index().size());
    parent_set.Set((i + 1) % index.index().size(), true);
    caches.emplace_back();
    caches.back().Insert(parent_set, bayes::branch_and_bound::CacheEntry(0, 0));
    cp_tables.emplace_back(i, parent_set);
    cp_tables.back().Build(index);
    cout << i << endl;
    cout << cp_tables.back().ToString() << endl;
    i++;
  }
  vector<bayes::branch_and_bound::Variable> variables;
  bayes::branch_and_bound::Variable::InitializeVariables(index, variables,
                                                         caches, cp_tables);
  bayes::branch_and_bound::Graph graph(variables);
  bayes::branch_and_bound::inference::Ranker ranker(graph, disc);
  i = 0;
  while (i < instances.size()) {
    cout << instances[i].ToString()
         << " | score =  " << ranker.Rank(instances[i]) << endl;
    i++;
  }
  return 0;
}
