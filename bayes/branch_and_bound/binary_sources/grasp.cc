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
  
  bayes::branch_and_bound::Instance::ParseDataset(input_file_path, disc,
                                                  instances);
  
  bayes::branch_and_bound::InvertedIndex index(instances);
  //instances.clear();
  vector<bayes::branch_and_bound::Cache> caches;
  bayes::branch_and_bound::Cache::InitializeCaches(
      cache_directory, instances, caches, 100000000);
  vector<bayes::branch_and_bound::ExternalQueue> external_queues;
  vector<bayes::branch_and_bound::Variable> variables;
  
  bayes::branch_and_bound::Variable::InitializeVariables(
      index, variables, external_queues, caches);
  
  bayes::branch_and_bound::heuristic::Grasp grasp(variables, &index);
  
  cout << "Running Grasp\n";
  bayes::branch_and_bound::Graph best = grasp.run(2, 10);
  std::cout << "Best graph is:" << std::endl
            << best.ToString() << std::endl;
  return 0;
}


