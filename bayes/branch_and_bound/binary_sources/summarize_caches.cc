#include "../variable.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <sstream>
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
  std::cerr << "Reading dataset" << std::endl;
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
  index.clear();
  std::cerr << "Done" << std::endl;

  unsigned var_count = 0;
  std::string cache_str;
  auto var = variables.cbegin();
  while (var != variables.cend()) {
    var_count += !var->cache()->cache().empty();
    if (!var->cache()->cache().empty()) {
      stringstream id;
      id << var->variable_id() << " " << var->cache()->ToString();
      cache_str += id.str();
    }
    ++var;
  }
  std::cout << var_count << std::endl;
  std::cout << cache_str;
  return 0;
}
