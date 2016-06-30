#include "../cache_builder.h"
#include "../../../base/dataset.h"
#include "../../../util/discretizer.h"
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
  if (argc < 8) {
    cerr << "[input file] [cache directory] [memory cache size] [temporary "
            "queue directory] [memory queue size] [bin count] "
            "[criterion (0 -> AIC), (1 -> BIC)]"
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
  bayes::branch_and_bound::Criterion criterion =
      (argv[7][0] == '0')
          ? bayes::branch_and_bound::Criterion::AKAIKE_INFORMATION_CRITERION
          : bayes::branch_and_bound::Criterion::MINIMUM_DESCRIPTION_LEGNTH;
  util::Discretizer disc(util::Discretizer::Mode::TREE_BASED_UNSUPERVISED,
                         bin_count);
  vector<bayes::branch_and_bound::Instance> instances;
  bayes::branch_and_bound::Instance::ParseDataset(input_file_path, disc,
                                                  instances);
  vector<bayes::branch_and_bound::Cache> caches;
  bayes::branch_and_bound::Cache::InitializeCaches(
      cache_directory, instances, caches, cache_size, criterion);
  bayes::branch_and_bound::InvertedIndex index(instances);
  instances.clear();
  vector<bayes::branch_and_bound::ExternalQueue> external_queues;
  bayes::branch_and_bound::ExternalQueue::InitializeExternalQueues(
      queue_directory, index.index().size(), queue_size, external_queues);
  vector<bayes::branch_and_bound::Variable> variables;
  vector<bayes::branch_and_bound::inference::CPTable> cp_tables;
  bayes::branch_and_bound::Variable::InitializeVariables(index, variables,
                                                         caches, cp_tables);
  unsigned id = 0;
  while (id < variables.size()) {
    bayes::branch_and_bound::CacheBuilder builder(&variables[id],
                                                  &external_queues[id]);
    builder.Build(index, variables);
    bayes::branch_and_bound::ExternalQueue::ClearExternalQueue(queue_directory, id);
    id++;
  }
  return 0;
}
