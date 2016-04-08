#include <cassert>
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
#include "../index_variable.h"
#include "../../../base/dataset.h"
#include "../../../util/discretizer.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc < 6) {
    cerr << "[input file] [cache directory] [memory cache size] [temporary "
            "queue directory] [memory queue size] [bin count]"
         << endl;
    return 0;
  }
  string input_file_path = argv[1], cache_directory = argv[2],
         queue_directory = argv[4];
  size_t cache_size = 0, queue_size = 0;
  unsigned bin_count = 0;
  sscanf(argv[3], "%zu", &cache_size);
  sscanf(argv[5], "%zu", &queue_size);
  sscanf(argv[6], "%u", &bin_count);
  util::Discretizer disc(util::Discretizer::Mode::TREE_BASED_UNSUPERVISED,
                         bin_count);
  vector<bayes::branch_and_bound::Instance> instances;
  bayes::branch_and_bound::Instance::ParseDataset(input_file_path, disc,
                                                  instances);
  vector<bayes::branch_and_bound::Cache> caches;
  bayes::branch_and_bound::Cache::InitializeCaches(cache_directory, instances,
                                                   caches, cache_size);
  vector<bayes::branch_and_bound::ExternalQueue> external_queues;
  bayes::branch_and_bound::ExternalQueue::InitializeExternalQueues(
      queue_directory, instances, queue_size, external_queues);
  bayes::branch_and_bound::InvertedIndex index(instances);
  instances.clear();
  vector<bayes::branch_and_bound::Variable> variables;
  bayes::branch_and_bound::Variable::InitializeVariables(
      index, variables, external_queues, caches);
  auto it = variables.begin();
  while (it != variables.end()) {
    it->BuildCache(index, variables);
    ++it;
  }
  return 0;
}
