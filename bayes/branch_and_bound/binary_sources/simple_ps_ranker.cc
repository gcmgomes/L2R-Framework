// This piece of code receives a test file and a cache with a bunch of
// parent sets. Then, for each parent set it classifies all entries from the
// test file.
//
// The output of this script follows a format similar to the cache, but for
// every parent set a MAP is printed as well.


#include "../../../base/dataset.h"
#include "../../../bayes/branch_and_bound/cache.h"
#include "../../../bayes/branch_and_bound/graph.h"
#include "../../../bayes/branch_and_bound/variable.h"
#include "../../../bayes/branch_and_bound/inference/ranker.h"
#include "../../../bayes/branch_and_bound/inference/cp_table.h"
#include "../../../util/file.h"
#include "../../../util/error_output.h"
#include <algorithm>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;


// Method that selects the instances of a given query.
// If the query_number is negative, the entire set of instances is returned.
vector<bayes::branch_and_bound::Instance> FilterInstances(
                    const vector<bayes::branch_and_bound::Instance>& instances,
                    int query_number)
{
  if(query_number < 0) return instances;
  
  vector<bayes::branch_and_bound::Instance> filtered;
  for(bayes::branch_and_bound::Instance inst : instances) {
    if((int)inst.query_id() == query_number) {
      filtered.push_back(inst);
    }
  }
  return filtered;
}


// This method returns a list of labels given to each document.
std::vector<double> run(const bayes::branch_and_bound::Bitset& parent_set,
                    const vector<bayes::branch_and_bound::Instance>& instances,
                    const bayes::branch_and_bound::InvertedIndex& index,
                    std::vector<bayes::branch_and_bound::Cache>& caches)
{
  vector<bayes::branch_and_bound::Variable> variables;
  vector<bayes::branch_and_bound::inference::CPTable> cp_tables;
  cp_tables.push_back(
      bayes::branch_and_bound::inference::CPTable(0, parent_set));
  cp_tables[0].Build(index);
  bayes::branch_and_bound::Variable::InitializeVariables(index, variables,
                                                         caches, cp_tables);
  bayes::branch_and_bound::Graph graph(variables);
  
  for(unsigned i = 0; i < parent_set.bit_count(); i++) {
    if(parent_set.at(i))
      graph.AddArc(i, 0, &index);
  }
  
  util::Discretizer disc(util::Discretizer::Mode::TREE_BASED_UNSUPERVISED,
                         20);

  // Since we don't use the Rank function passing a document,
  // it doesn't matter the discretizer I pass in here.
  bayes::branch_and_bound::inference::Ranker ranker(graph, disc);
  
  // Keeps the ranking value for each instance. 
  std::vector<double> v_ranker;
  
  for(bayes::branch_and_bound::Instance inst : instances) {
    v_ranker.push_back(ranker.Rank(inst));
  }
  
  return v_ranker;
}


int main(int argc, char* argv[])
{
  if(argc != 7)
  {
    cerr << "Usage:\n    ./" << argv[0]
         << " [train filename] [query number -> -1 if all queries]"
         << " [test filename] [label cache directory] [output path]"
         << " [criterion (0 -> AIC), (1 -> BIC)]\n";
    return 1;
  }
  
  string train_filename = argv[1];
  int query_number = -1, criterion=-1;
  string test_filename = argv[3], label_cache_directory = argv[4];
  string output_path = argv[5];
  
  sscanf(argv[2], "%d", &query_number);
  sscanf(argv[6], "%d", &criterion);

  if(*output_path.rbegin() != '/') {
    output_path += "/";
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
  std::vector<bayes::branch_and_bound::Cache> caches;

  std::cerr << "Parsing Test Dataset...\n";
  bayes::branch_and_bound::Instance::ParseDataset(test_filename,
                                                  test_instances);

  std::cerr << "Parsing Train Dataset...\n";
  bayes::branch_and_bound::Instance::ParseDataset(train_filename,
                                                  train_instances);

  train_instances = FilterInstances(train_instances, query_number);
  bayes::branch_and_bound::InvertedIndex index(train_instances);


  std::cerr << "Loading caches...\n";
  bayes::branch_and_bound::Cache::LoadCaches(label_cache_directory,
                                             train_instances[0].values().size(),
                                             caches);
  
  // Keeps the id of the current parent set.
  int n_parent_set = 1;

  std::cerr << "Ranking Documents...\n";
  for(auto entry : caches[0].cache())
  {
    std::vector<double> v_ranker = run(entry.first, test_instances, index,
                                      caches);
    
    // Printing the output...
    std::fstream outstream;
    stringstream filename;
    filename << output_path+"results_";
    filename << n_parent_set++;
    outstream.open(filename.str(), std::fstream::out);
    outstream << std::setprecision(6) << std::fixed;
    
    for(double value : v_ranker) {
      outstream << value << std::endl;
    }
    
    // For AIC w = 1.
    // For BIC, w = log2(n) / 2.
    long double w = 1, score=0;
    if(criterion == 1) {
      w = log2(test_instances.size()) / 2.0;
    }
    score = entry.second.score(w);
    
    outstream << entry.first.bit_string() << " " << score << std::endl;
    outstream.close();
  }
  
  
  return 0; 
}
