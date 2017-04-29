#ifndef _ranker_helper
#define _ranker_helper

#include "../../../base/dataset.h"
#include "../../../bayes/branch_and_bound/cache.h"
#include "../../../bayes/branch_and_bound/graph.h"
#include "../../../bayes/branch_and_bound/inference/ranker.h"
#include "../../../bayes/branch_and_bound/inference/cp_table.h"
#include <algorithm>
#include <cstdlib>
#include <vector>

namespace bayes {
namespace branch_and_bound {

  // Method that selects the instances of a given query.
  // If the query_number is negative, the entire set of instances is returned.
  std::vector<bayes::branch_and_bound::Instance> FilterInstances(
      const std::vector<bayes::branch_and_bound::Instance>& instances,
      int query_number);


  // This method returns a list of labels given to each document.
  std::vector<double> run(const bayes::branch_and_bound::Bitset& parent_set,
      const std::vector<bayes::branch_and_bound::Instance>& instances,
      const bayes::branch_and_bound::InvertedIndex& index,
      std::vector<bayes::branch_and_bound::Cache>& caches);
} // namespace branch_and_bound
} // namespace bayes

#endif
