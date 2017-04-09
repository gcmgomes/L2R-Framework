// This piece of code receives a test file and a cache with a bunch of
// parent sets. Then, for each parent set it classifies all entries from the
// test file.
//
// The output of this script follows a format similar to the cache, but for
// every parent set a MAP is printed as well.


#include "../../../base/dataset.h"
#include "../../../bayes/branch_and_bound/cache.h"
#include "../../../bayes/branch_and_bound/bitset.h"
#include "../../../bayes/branch_and_bound/inference/cp_table.h"
#include "../../../util/file.h"
#include "../../../util/error_output.h"
#include "../../../util/statistics.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;

typedef struct sOutput {
  long double parent_set_score;
  double MAP;
  bayes::branch_and_bound::Bitset bitset;
  
  sOutput() {}
  sOutput(long double parent_set_score_, double MAP_,
          bayes::branch_and_bound::Bitset bitset_)
  {
    parent_set_score = parent_set_score_;
    MAP = MAP_;
    bitset = bitset_;
  }
} output;

// This methods calculates the MAP@10 given a parent set.
double run(const bayes::branch_and_bound::Bitset& parent_set,
           const vector<bayes::branch_and_bound::Instance>& instances)
{
  bayes::branch_and_bound::inference::CPTable cp_table(0, parent_set);
  
  // Keeps pairs <classification, expectation>
  std::vector<std::pair<double, unsigned> > v_classification;
  
  for(bayes::branch_and_bound::Instance inst : instances)
  {
    v_classification.push_back({cp_table.Query(inst), inst.values()[0]});
  }
  
  return util::Statistics::Mapk(v_classification, 10u);
}


int main(int argc, char* argv[])
{
  if(argc != 4)
  {
    cerr << "Usage:\n    ./" << argv[0]
         << " [test filename] [label cache directory] [output path]\n";
    return 1;
  }

  string test_filename = argv[1], label_cache_directory = argv[2];
  string output_path = argv[3];
  
  if(*output_path.rbegin() != '/')
  {
    output_path += "/";
  }

  // Making sure the output path exists...
  // And create it if not.
  if(!util::File::CreateDirectory(output_path))
  {
    util::Error::Print("Could not find nor create the output path.");
    return 1;
  }
  
  vector<bayes::branch_and_bound::Instance> instances;
  std::vector<bayes::branch_and_bound::Cache> caches;
  
  std::cerr << "Parsing Test Dataset...\n";
  bayes::branch_and_bound::Instance::ParseDataset(test_filename,
                                                  instances);
  std::cerr << "Loading caches...\n";
  bayes::branch_and_bound::Cache::LoadCaches(label_cache_directory,
                                             1, caches);
  
  vector<output> v_output;
  std::cerr << "Calculating MAP for every parent set...\n";
  // Calculate MAP for every parent set of the label.
  for(auto entry : caches[0].cache())
  {
    double MAP = run(entry.first, instances);
    
    // Passing w=1 to score, that means that the score is calculated as AIC.
    // TODO: Change that, so we can run experiments with BIC as well.
    v_output.push_back(sOutput(entry.second.score(1), MAP, entry.first));
  }
  
  std::cerr << "Printing output...\n";
  std::fstream outstream;
  outstream.open(output_path+"results", std::fstream::out);
  outstream << std::setprecision(6) << std::fixed;
  outstream << v_output.size() << std::endl;
  for(output out : v_output)
  {
    outstream << std::fixed << out.bitset.bit_string() << " "
              << out.parent_set_score << " "
              << out.MAP << std::endl;
  }
  outstream.close();
  
  return 0; 
}
