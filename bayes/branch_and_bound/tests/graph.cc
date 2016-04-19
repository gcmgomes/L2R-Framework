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
#include "../graph.h"

using namespace std;

int main(int argc, char** argv) {
  string file_path = "/var/tmp/data.txt";
  if (argc > 1) {
    file_path = argv[1];
  }
  util::Discretizer disc(util::Discretizer::Mode::TREE_BASED_UNSUPERVISED, 2);
  vector<bayes::branch_and_bound::Instance> instances;
  bayes::branch_and_bound::Instance::ParseDataset(file_path, disc, instances);
  bayes::branch_and_bound::InvertedIndex index(instances);
  instances.clear();
  vector<bayes::branch_and_bound::Cache> caches;
  int abcd = 0;
  while(abcd < 4) {
    using namespace bayes::branch_and_bound;
    caches.emplace_back(1, -1);
    Bitset bitset(4);
    bitset.Set((abcd+1) % 4, true);
    caches[abcd].Insert(bitset, CacheEntry(-10, 10));
    abcd++;    
  }
  vector<bayes::branch_and_bound::ExternalQueue> external_queues;
  vector<bayes::branch_and_bound::Variable> variables;
  bayes::branch_and_bound::Variable::InitializeVariables(
      index, variables, external_queues, caches);

  vector<pair<unsigned, unsigned>> edges;
  edges.push_back(make_pair(1, 0));
  edges.push_back(make_pair(2, 1));
  edges.push_back(make_pair(3, 2));
  edges.push_back(make_pair(0, 3));
  auto it = variables.begin();
  while (it != variables.end()) {
    it->mutable_parent_set().clear();
    ++it;
  }
  unsigned i = 0;
  while (i < edges.size()) {
    variables[edges[i].second].mutable_parent_set().Set(edges[i].first, true);
    i++;
  }
  bayes::branch_and_bound::Graph g(variables);
  g.ReadyForUse(variables);
  std::cerr << g.ToString() << std::endl;
  vector<unsigned> cycle;
  g.FindCycle(cycle);
  i = 0;
  while (i < cycle.size()) {
    if (i) {
      std::cerr << " -> ";
    }
    std::cerr << cycle[i];
    i++;
  }
  std::cerr << std::endl;
  return 0;
}
