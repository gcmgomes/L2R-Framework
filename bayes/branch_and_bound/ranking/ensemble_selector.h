#ifndef _bayes_branch_and_bound_ranking_ensemble_selector_
#define _bayes_branch_and_bound_ranking_ensemble_selector_

#include <functional>
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
    std::vector<Bitset> random(
        std::vector<std::vector<Cache>*> caches,
        long double &ensemble_score, int criterion, int test_instances_size);
    

    // Returns the worst ensemble of parent sets.
    // Returns the ensemble with the lowest sum of scores.
    std::vector<Bitset> minimum(
        std::vector<std::vector<Cache>*> caches,
        long double &ensemble_score, int criterion, int test_instances_size);

    // Returns the best ensemble of parent sets.
    // Returns the ensemble with the biggest sum of scores.
    std::vector<Bitset> maximum(
        std::vector<std::vector<Cache>*> caches,
        long double &ensemble_score, int criterion, int test_instances_size);

    // Selecting a parent set per query.
    // This method returns the k-th best ensemble.
    std::vector<Bitset> topk(
        std::vector<std::vector<Cache>*> caches);
    
  private:
    
    static std::pair<Bitset, CacheEntry>
    getMinimumParentSet(std::unordered_map<Bitset, CacheEntry>
                                       map_cache, int criterion,
                                       int test_instances_size,
                                       long double &ps_score);
    
    static std::pair<Bitset, CacheEntry>
    getMaximumParentSet(std::unordered_map<Bitset, CacheEntry>
                                       map_cache, int criterion,
                                       int test_instances_size,
                                       long double &ps_score);
    
    static std::pair<Bitset, CacheEntry>
    getRandomParentSet(std::unordered_map<Bitset, CacheEntry>
                                       map_cache, int criterion,
                                       int test_instances_size,
                                       long double &ps_score);
    
    std::vector<Bitset> selector(
        std::vector<std::vector<Cache>*> caches,
        long double &ensemble_score, int criterion, int test_instances_size,
        std::function<std::pair<Bitset, CacheEntry>(std::unordered_map<Bitset, CacheEntry>, int, int, long double&)> func);

};

} // namespace branch_and_bound
} // namespace bayes


#endif
