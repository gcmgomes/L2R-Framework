#include <cstdio>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include "../branch_and_bound.h"
#include "../heuristic/grasp.h"
using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 4) {
    cerr << argv[0] << " [input file] [cache directory] [bin count]" << endl;
    return 0;
  }
  string input_file_path = argv[1];  //, cache_directory = argv[2];
  string cache_directory = "/tmp";
  unsigned bin_count = 0;
  sscanf(argv[3], "%u", &bin_count);
  util::Discretizer disc(util::Discretizer::Mode::TREE_BASED_UNSUPERVISED,
                         bin_count);
  vector<bayes::branch_and_bound::Instance> instances;
  
  cout << "Parsing Dataset...\n";
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
  
  bayes::branch_and_bound::heuristic::Grasp grasp(variables, &index);
  
  cout << "Running Grasp\n";
  bayes::branch_and_bound::Graph best = grasp.Run(2, 100);
  std::cout << "Score of the Best Graph: " << best.score() << endl;
  std::cout << "Best graph is:" << std::endl
            << best.ToString() << std::endl;
  
  long double sum = 0;
  
  for(int i = 0; i < 30; i++)
  {
    best = grasp.Run(2, 100);
    sum += best.score();
  }
  
  cout << "Average Score for 30 runs: " << sum/30 << endl;
  return 0;
}


