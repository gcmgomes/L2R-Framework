#include <cstdio>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include "../branch_and_bound.h"
#include "../heuristic/pathrelinking.h"
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
  //instances.clear();
  vector<bayes::branch_and_bound::Cache> caches;
  bayes::branch_and_bound::Cache::InitializeCaches(
      cache_directory, instances, caches, 100000000);
  vector<bayes::branch_and_bound::Variable> variables;
  vector<bayes::branch_and_bound::inference::CPTable> cp_tables;
  bayes::branch_and_bound::Variable::InitializeVariables(
      index, variables, caches, cp_tables);
  
  bayes::branch_and_bound::heuristic::PathRelinking pathrelinking(variables, &index);
  
  cout << "Running Path-Relinking\n";
  bayes::branch_and_bound::Graph best = pathrelinking.run();
  std::cout << "Score of the Best Graph: " << best.score() << endl;
  std::cout << "Best graph is:" << std::endl
            << best.ToString() << std::endl;
  
  std::cout << "Calculating Average of 30 runs ...\n";
  long double sum = 0;
  for(int i = 0; i < 30; i++)
  {
    cout << "#" << i+1 << " Run.\n";
    best = pathrelinking.run();
    sum += best.score();
  }
  
  cout << "Average Score for 30 runs: " << sum/30 << endl;
  return 0;
}


