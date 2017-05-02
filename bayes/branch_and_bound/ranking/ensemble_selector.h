#ifndef _bayes_branch_and_bound_ranking_ensemble_selector_
#define _bayes_branch_and_bound_ranking_ensemble_selector_

#include <vector>

#include "../../../bayes/branch_and_bound/cache.h"


namespace bayes {
namespace branch_and_bound {

class EnsembleSelector {
  public:
    
    EnsembleSelector();

    // This method selects a parent set per query.
    // For each query, the parent set is chosen randomly.
    // The ensemble_score returned here is the mean of scores.
    // If the ensemble_score must be calculated as AIC, criterion must be 1.
    // If the ensemble_score must be calculated as BIC, criterion must be 0.
    std::vector<bayes::branch_and_bound::Bitset> random(
        std::vector<std::vector<bayes::branch_and_bound::Cache>*> caches,
        long double &ensemble_score, int criterion, int test_instances_size);
    
    // Selecting a parent set per query.
    // This method returns the k-th best ensemble.
    std::vector<bayes::branch_and_bound::Bitset> topk(
        std::vector<std::vector<bayes::branch_and_bound::Cache>*> caches);
    
  private:
  
};


} // namespace branch_and_bound
} // namespace bayes


#endif
