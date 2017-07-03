// This piece of code receives a test file and a cache with a bunch of
// parent sets. 

// Then, for each parent set it classifies all entries from the
// test file.
//
// The output of this script follows a format similar to the cache, but for
// every parent set a MAP is printed as well.
//
// The ranking is done by an ensemble.
// The ensemble is made by a set of parent sets;
// For each query at the test file, an parent set is picked.


#include "../../../base/dataset.h"
#include "../../../bayes/branch_and_bound/cache.h"
#include "../../../bayes/branch_and_bound/graph.h"
#include "../../../bayes/branch_and_bound/variable.h"
#include "../../../bayes/branch_and_bound/inference/ranker.h"
#include "../../../bayes/branch_and_bound/inference/cp_table.h"
#include "../../../util/file.h"
#include "../../../util/error_output.h"
#include "../ranking/ensemble_selector.h"
#include "../ranking/ranker_helper.h"
#include <algorithm>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>
#include <errno.h>
#include <vector>
using namespace std;
using namespace bayes::branch_and_bound;


int main(int argc, char* argv[])
{
  if(argc != 9)
  {
    cerr << "Usage:\n    ./" << argv[0]
         << " [train filename] [query number -> -1 if all queries]"
         << " [test filename] [label cache directory] [output path]"
         << " [criterion (0 -> AIC), (1 -> BIC)]"
         << " [Number of times to be run]"
         << " [0 - Minimum PS; 1 - Maximum PS; 2 - Random PS's]\n";
    return 1;
  }
  
  string train_filename = argv[1];
  int query_number = -1, criterion=-1;
  string test_filename = argv[3], label_cache_directory = argv[4];
  string output_path = argv[5];
  int run_times, operation;

  sscanf(argv[2], "%d", &query_number);
  sscanf(argv[6], "%d", &criterion);
  sscanf(argv[7], "%d", &run_times);
  sscanf(argv[8], "%d", &operation);

  if(*output_path.rbegin() != '/') {
    output_path += "/";
  }
  
  if(*label_cache_directory.rbegin() != '/') {
    label_cache_directory += "/";
  }

  // Making sure the output path exists...
  // And create it if not.
  if(!util::File::CreateDirectory(output_path))
  {
    util::Error::Print("Could not find nor create the output path.");
    return 1;
  }

  vector<bayes::branch_and_bound::Instance> test_instances;
  vector<bayes::branch_and_bound::Instance> train_instances;

  std::cerr << "Parsing Test Dataset...\n";
  bayes::branch_and_bound::Instance::ParseDataset(test_filename,
                                                  test_instances);

  std::cerr << "Parsing Train Dataset...\n";
  bayes::branch_and_bound::Instance::ParseDataset(train_filename,
                                                  train_instances);

  bayes::branch_and_bound::InvertedIndex index(train_instances);
 

  EnsembleSelector selector;
  
  std::cerr << "Loading caches...\n";
  std::vector<std::vector<bayes::branch_and_bound::Cache>*> caches;

  for(unsigned query_id = 0; query_id < 50; query_id++)
  {
    stringstream query_cache_directory;
    query_cache_directory << label_cache_directory;
    query_cache_directory << "query";
    query_cache_directory << query_id;
    
    if(!util::File::DirectoryExists(query_cache_directory.str()))
      continue;
    
    std::vector<Cache> *query_cache = new std::vector<Cache>;
    Cache::LoadCaches(query_cache_directory.str(),
                      train_instances[0].values().size(),
                      *query_cache);
    caches.push_back(query_cache);
  }
  
  std::cerr << "\nRanking Documents...\n";
  for(int i = 0; i < run_times; i++)
  {
    long double ensemble_score = 0;
    std::vector<bayes::branch_and_bound::Bitset> ensemble;
    
    std::ofstream outstream;
    stringstream filename;
    filename << output_path+"ensemble_ps_results_";
    
    // Getting the ensemble...
    switch(operation) {
      case 0:
        ensemble = selector.minimum(caches, ensemble_score,
                                    criterion, test_instances.size());
        filename << "minimum_";
        break;
      case 1:
        ensemble = selector.maximum(caches, ensemble_score,
                                    criterion, test_instances.size());
        filename << "maximum_";
        break;
      default:
        ensemble = selector.random(caches, ensemble_score,
                                   criterion, test_instances.size());
        filename << "random_";
        break;
    }
    filename << i;
    
    
    std::vector<double> v_ranker = run(ensemble, test_instances, index,
                                      caches);
    
    // Printing the output...
    std::cerr << "Output: " << filename.str() << std::endl;
    outstream.open(filename.str(), std::ofstream::out);
    if ( (outstream.rdstate() & std::fstream::failbit ) != 0 ) {
      util::Error::Print(strerror(errno));
    }
    outstream << std::setprecision(6) << std::fixed;
    
    for(double value : v_ranker) {
      outstream << value << std::endl;
    }
    
    
    // TODO: On this line, print the position of the position
    // of the parent set as well.
    outstream << ensemble_score << std::endl;
    outstream.close();
  } 
  
  return 0; 
}
