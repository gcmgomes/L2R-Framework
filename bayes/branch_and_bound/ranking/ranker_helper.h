#ifndef _bayes_branch_and_bound_ranking_ranker_helper_
#define _bayes_branch_and_bound_ranking_ranker_helper_

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

  // Ranks using an ensemble of parent sets...
  // This method returns a list of labels given to each document.
  // The label given to each parent set is the mean of labels given per
  // parent set.
  // This method receives a cache for each query...
  std::vector<double> run(const std::vector<bayes::branch_and_bound::Bitset>&
      parent_sets,
      const std::vector<bayes::branch_and_bound::Instance>& instances,
      const bayes::branch_and_bound::InvertedIndex& index,
      std::vector<std::vector<bayes::branch_and_bound::Cache>*>& caches);

  // This method returns the position parent_set occupies at the cache.
  // Considering the first parent set is the one with the lowest score.
  std::vector<bayes::branch_and_bound::Bitset> findIndex(
      const bayes::branch_and_bound::Bitset& parent_sets,
      const std::vector<bayes::branch_and_bound::Instance>& instances,
      const bayes::branch_and_bound::InvertedIndex& index,
      std::vector<std::vector<bayes::branch_and_bound::Cache>*>& caches);

  // This method returns the position the ensemble of parent sets
  // occupies, considering all possible ensembles.
  // The first ensemble is the one with the lowest score.
  std::vector<bayes::branch_and_bound::Bitset> findIndex(
      const std::vector<bayes::branch_and_bound::Bitset>& parent_sets,
      const std::vector<bayes::branch_and_bound::Instance>& instances,
      const bayes::branch_and_bound::InvertedIndex& index,
      std::vector<std::vector<bayes::branch_and_bound::Cache>*>& caches);


} // namespace branch_and_bound
} // namespace bayes

#endif
