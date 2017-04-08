// Per Query Cache Builder
//
// It generates a cache per query and just for the label.

#include "../cache_builder.h"
#include "../../../base/dataset.h"
#include "../../../util/discretizer.h"
#include "../../../util/file.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int main(int argc, char** argv) {
  if (argc < 8) {
    cerr << ("[input file] [cache directory] [memory cache size] [temporary "
            "queue directory] [memory queue size] [bin count] "
            "[criterion (0 -> AIC), (1 -> BIC)]")
         << endl;
    return 1;
  }
  string input_file_path = argv[1], cache_directory = argv[2],
         queue_directory = argv[4];
  size_t cache_size = 0, queue_size = 0;
  unsigned bin_count = 0;
  sscanf(argv[3], "%zu", &cache_size);
  sscanf(argv[5], "%zu", &queue_size);
  sscanf(argv[6], "%u", &bin_count);
 
  if(*cache_directory.rbegin() != '/')
    cache_directory += '/';

  if(*queue_directory.rbegin() != '/')
    queue_directory += '/';

  // Creating cache and queue directories.
  if(!util::File::CreateDirectory(cache_directory))
  {
    std::cerr << "\e[1;31m";  // ANSI light red code.
    std::cerr << "Error: ";
    std::cerr << "\e[0m"; // ANSI black code.
    
    std::cerr << "Could not create cache directory\n";
  }

  if(!util::File::CreateDirectory(queue_directory))
  {
    std::cerr << "\e[1;31m";  // ANSI light red code.
    std::cerr << "Error: ";
    std::cerr << "\e[0m"; // ANSI black code.
    
    std::cerr << "Could not create temporary queue directory\n";
  }

  bayes::branch_and_bound::Criterion criterion =
      (argv[7][0] == '0')
          ? bayes::branch_and_bound::Criterion::AKAIKE_INFORMATION_CRITERION
          : bayes::branch_and_bound::Criterion::MINIMUM_DESCRIPTION_LEGNTH;
  util::Discretizer disc(util::Discretizer::Mode::TREE_BASED_UNSUPERVISED,
                         bin_count);
  vector<bayes::branch_and_bound::Instance> instances;
  bayes::branch_and_bound::Instance::ParseDataset(input_file_path, disc,
                                                  instances);
  
  // Separating the instances by query ids.
  map<unsigned, vector<bayes::branch_and_bound::Instance> > queries;
  for(auto inst : instances)
  {
    queries[inst.query_id()].push_back(inst); 
  }

  for(auto q : queries)
  {
    vector<bayes::branch_and_bound::Instance> &query_instances = q.second;
    
    // Initializing everything.
    vector<bayes::branch_and_bound::Cache> caches;
    
    bayes::branch_and_bound::InvertedIndex index(query_instances);
    
    // Ignoring queries that don't have multiple relevance labels.
    if(index.at(0).entries().size() < 2) continue;
    
    stringstream ss;
    ss << "query" << q.first;
    
    std::string dir = cache_directory+ss.str();
    util::File::CreateDirectory(dir);
    
    bayes::branch_and_bound::Cache::InitializeCaches(
        dir, query_instances, caches, cache_size, criterion);
    
    
    query_instances.clear();
    vector<bayes::branch_and_bound::ExternalQueue> external_queues;
    bayes::branch_and_bound::ExternalQueue::InitializeExternalQueues(
        queue_directory, index.index().size(), queue_size, external_queues);
    vector<bayes::branch_and_bound::Variable> variables;
    vector<bayes::branch_and_bound::inference::CPTable> cp_tables;
    bayes::branch_and_bound::Variable::InitializeVariables(index, variables,
        caches, cp_tables);
    
    
    // Building the cache just for the label
    unsigned id = 0;
    bayes::branch_and_bound::CacheBuilder builder(&variables[id],
        &external_queues[id]);
    builder.Build(index, variables);
    bayes::branch_and_bound::ExternalQueue::
      ClearExternalQueue(queue_directory, id);
  }

  return 0;
}
