// The final piece of the any-time ExternalBranch and Bound algorithm that
// discovers the
// optimal topology for a Bayesian Network for a given dataset.
#ifndef _L2RF_BAYES_BRANCH_AND_BOUND_HEURISTICS_GREEDY_LOCAL_SEARCH_H_
#define _L2RF_BAYES_BRANCH_AND_BOUND_HEURISTICS_GREEDY_LOCAL_SEARCH_H_

#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>
#include "../../graph.h"

namespace bayes {
namespace branch_and_bound {
namespace heuristics {

class GreedyLocalSearch {
 public:
  // Assume that |seed_| is a DAG.
  GreedyLocalSearch(const bayes::branch_and_bound::Graph& seed);

  // Greedily augments |seed| until it can no longer be improved or |max_rounds|
  // of augmentation were performed. Returns true if succesful, false otherwise.
  bool Run(unsigned max_rounds);

  const bayes::branch_and_bound::Graph& seed() {
    return seed_;
  }

 private:
  // Returns the best score in the cache of |variable_id| that preserves
  // acyclicity of |seed|. Stores the resulting bitset in |best_parent_set|.
  long double FindBestAugmentingEntry(
      unsigned variable_id, bayes::branch_and_bound::Bitset& best_parent_set);

  // Performs one round of augmentation on |seed_|. Returns true if succesful.,
  // false otherwise.
  bool Augment();

  bayes::branch_and_bound::Graph seed_;
};

}  // namespce heuristics
}  // namespce branch_and_bound
}  // namespace bayes

#endif
