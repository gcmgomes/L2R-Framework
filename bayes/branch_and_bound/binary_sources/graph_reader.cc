#include "../branch_and_bound.h"
#include "../graph.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main(int argc, char *argv[]) {
  cout << "BEGIN!\n";
  if (argc < 4) {
    cerr << argv[0] << " [input file] [cache directory] [bin count]" << endl;
    return 0;
  }
  string input_file_path = argv[1];  //, cache_directory = argv[2];
  string cache_directory = "/tmp";
  unsigned bin_count = 0;
  sscanf(argv[3], "%u", &bin_count);

  std::cout << "Initializing Discretizer...\n";
  util::Discretizer disc(util::Discretizer::Mode::TREE_BASED_UNSUPERVISED,
                         bin_count);
  vector<bayes::branch_and_bound::Instance> instances;

  std::cout << "Parsing Dataset...\n";
  bayes::branch_and_bound::Instance::ParseDataset(input_file_path, disc,
                                                  instances);

  bayes::branch_and_bound::InvertedIndex index(instances);
  // instances.clear();
  vector<bayes::branch_and_bound::Cache> caches;
  bayes::branch_and_bound::Cache::InitializeCaches(cache_directory, instances,
                                                   caches, 100000000);
  vector<bayes::branch_and_bound::Variable> variables;
  vector<bayes::branch_and_bound::inference::CPTable> cp_tables;
  bayes::branch_and_bound::Variable::InitializeVariables(index, variables,
                                                         caches, cp_tables);

  
  int n, id;
  string parentset;
  cin >> n;
  
  variables.clear();
  for(int i = 0; i < n; i++)
    variables.push_back(bayes::branch_and_bound::Variable(i, bayes::branch_and_bound::Bitset()));
  
  while(n--)
  {
    cin >> id;
    cin >> parentset;
    
    for(unsigned i = 0; i < parentset.size(); i++)
      if(parentset[i]=='1')
        variables[i].mutable_parent_set().Set(i, true);
    
    cin >> id;
  }
  
  bayes::branch_and_bound::Graph g(variables);
  vector<unsigned> fudeu;
  g.FindCycle(fudeu);
  if(fudeu.size()) cout << "FUDEU\n";
  else cout << "FUDEO NAO - HEUAHUEAHUEHUAEHEAUHEUEEHAUEAH\n";
  
  return 0;
}
