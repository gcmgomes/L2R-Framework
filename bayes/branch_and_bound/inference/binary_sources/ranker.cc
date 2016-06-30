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
  if (argc < 6) {
    cerr << argv[0] << " [train file] [cache directory] [bin count] [model "
                       "file] [test file] "
         << endl;
    return 0;
  }
  string train_file_path = argv[1], cache_directory = argv[2],
         model_file_path = argv[4], test_file_path = argv[5];
  unsigned bin_count = 0;
  sscanf(argv[3], "%u", &bin_count);
  util::Discretizer disc(util::Discretizer::Mode::TREE_BASED_UNSUPERVISED,
                         bin_count);
  vector<bayes::branch_and_bound::Instance> instances;
  bayes::branch_and_bound::Instance::ParseDataset(train_file_path, disc, instances);
  bayes::branch_and_bound::InvertedIndex index(instances);
  instances.clear();
  vector<bayes::branch_and_bound::Cache> caches;
  bayes::branch_and_bound::Cache::LoadCaches(cache_directory,
                                             index.index().size(), caches);
  std::vector<bayes::branch_and_bound::inference::CPTable> cp_tables;
  unsigned i = 0;
  while (i < index.index().size()) {
    bayes::branch_and_bound::Bitset parent_set(index.index().size());
    cp_tables.emplace_back(i, parent_set);
    i++;
  }
  vector<bayes::branch_and_bound::Variable> variables;
  bayes::branch_and_bound::Variable::InitializeVariables(index, variables,
                                                         caches, cp_tables);
  bayes::branch_and_bound::Graph graph(variables);
  graph.FromFile(model_file_path);
  i = 0;
  while (i < cp_tables.size()) {
    cp_tables[i].Rebuild(index, graph.variables().at(i).parent_set());
    i++;
  }
  bayes::branch_and_bound::inference::Ranker ranker(graph, disc);

  base::Dataset test_set;
  test_set.Parse(test_file_path, true);
  auto query_it = test_set.cbegin();
  while(query_it != test_set.cend()) {
    auto doc_it = query_it->cbegin();
    while(doc_it != query_it->cend()) {
      cout << ranker.Rank(*doc_it) << endl;
      ++doc_it;
    }
    ++query_it;
  }
  return 0;
}
