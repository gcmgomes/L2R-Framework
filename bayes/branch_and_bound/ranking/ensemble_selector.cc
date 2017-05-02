#include "ensemble_selector.h"

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <algorithm>
#include <unordered_map>

namespace bayes {
namespace branch_and_bound {

  EnsembleSelector::EnsembleSelector() {
    srand(time(NULL));
  }

  // This method selects a parent set per query.
  // For each query, the parent set is chosen randomly.
  std::vector<bayes::branch_and_bound::Bitset> EnsembleSelector::random(
      std::vector<std::vector<bayes::branch_and_bound::Cache>*> caches,
      long double &ensemble_score, int criterion, int test_instances_size) {
    std::vector<bayes::branch_and_bound::Bitset> ensemble;
    ensemble_score=0;

    for (std::vector<bayes::branch_and_bound::Cache>* query_cache : caches) {
      if(query_cache == NULL)
        continue;
      
      std::unordered_map<bayes::branch_and_bound::Bitset,
                         bayes::branch_and_bound::CacheEntry>
                         map_cache = (*query_cache)[0].cache();
      
      if(map_cache.size() == 0)
        continue;
      
      int random_index = rand()%map_cache.size();
      auto random_it = std::next(std::begin(map_cache), random_index);
    
      // For AIC w = 1.
      // For BIC, w = log2(n) / 2.
      long double w = 1;
      if(criterion == 1) {
        w = log2(test_instances_size) / 2.0;
      }
      ensemble_score += random_it->second.score(w);
      ensemble.push_back(random_it->first);
    }
    
    ensemble_score /= ensemble.size();

    return ensemble;
  }

  // Selecting a parent set per query.
  // This method returns the k-th best ensemble.
  std::vector<bayes::branch_and_bound::Bitset> EnsembleSelector::topk(
      std::vector<std::vector<bayes::branch_and_bound::Cache>*> caches) {
    // TODO: Throw an YetToBeImplemented Exception.
    return std::vector<bayes::branch_and_bound::Bitset>();
  }


} // namespace branch_and_bound
} // namespace bayes
