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
    return this->selector(caches, ensemble_score, criterion,
                          test_instances_size, EnsembleSelector::getRandomParentSet);
  }
  
  // Returns the worst ensemble of parent sets.
  // Returns the ensemble with the lowest sum of scores.
  std::vector<bayes::branch_and_bound::Bitset> EnsembleSelector::minimum(
      std::vector<std::vector<bayes::branch_and_bound::Cache>*> caches,
      long double &ensemble_score, int criterion, int test_instances_size) {
    return this->selector(caches, ensemble_score, criterion,
                          test_instances_size, EnsembleSelector::getMinimumParentSet);
  }
  
  // Returns the best ensemble of parent sets.
  // Returns the ensemble with the biggest sum of scores.
  std::vector<bayes::branch_and_bound::Bitset> EnsembleSelector::maximum(
      std::vector<std::vector<bayes::branch_and_bound::Cache>*> caches,
      long double &ensemble_score, int criterion, int test_instances_size) {
    return this->selector(caches, ensemble_score, criterion,
                          test_instances_size, EnsembleSelector::getMaximumParentSet);
  }
  

  std::pair<Bitset, CacheEntry>
  EnsembleSelector::getMinimumParentSet(std::unordered_map<Bitset, CacheEntry>
                                       map_cache, int criterion,
                                       int test_instances_size,
                                       long double &ps_score) {
    long double score = 0x3f3f3f3f;
    std::pair<Bitset, CacheEntry> ps;
    for(auto it : map_cache)
    {
      // For AIC w = 1.
      // For BIC, w = log2(n) / 2.
      long double w = 1;
      if(criterion == 1) {
        w = log2(test_instances_size) / 2.0;
      }
      long double tmp_score = it.second.score(w);
      
      if(tmp_score < score) {
        score = tmp_score;
        ps = it;
      }
    }
    ps_score = score;
    return ps;
  }
  
  std::pair<Bitset, CacheEntry>
  EnsembleSelector::getMaximumParentSet(std::unordered_map<Bitset, CacheEntry>
                                       map_cache, int criterion,
                                       int test_instances_size,
                                       long double &ps_score) {
    long double score = -0x3f3f3f3f;
    std::pair<Bitset, CacheEntry> ps;
    for(auto it : map_cache)
    {
      // For AIC w = 1.
      // For BIC, w = log2(n) / 2.
      long double w = 1;
      if(criterion == 1) {
        w = log2(test_instances_size) / 2.0;
      }
      long double tmp_score = it.second.score(w);
      
      if(tmp_score > score) {
        score = tmp_score;
        ps = it;
      }
    }
    ps_score = score;
    return ps;
  }

  std::pair<Bitset, CacheEntry>
  EnsembleSelector::getRandomParentSet(std::unordered_map<Bitset, CacheEntry>
                                       map_cache, int criterion,
                                       int test_instances_size,
                                       long double &ps_score) {
    int random_index = rand()%map_cache.size();
    auto random_it = std::next(std::begin(map_cache), random_index);

    // For AIC w = 1.
    // For BIC, w = log2(n) / 2.
    long double w = 1;
    if(criterion == 1) {
      w = log2(test_instances_size) / 2.0;
    }
    ps_score = random_it->second.score(w);
    
    return *random_it;
  }

  std::vector<Bitset> EnsembleSelector::selector(
        std::vector<std::vector<Cache>*> caches,
        long double &ensemble_score, int criterion,
        int test_instances_size,
        std::function<std::pair<Bitset, CacheEntry>(std::unordered_map<Bitset, CacheEntry>, int, int, long double&)> func) {
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
      
      long double ps_score=0;
      
      std::pair<Bitset, CacheEntry> ps = func(map_cache, criterion,
                                              test_instances_size, ps_score);
      ensemble_score += ps_score;
      ensemble.push_back(ps.first);
    }

    ensemble_score /= ensemble.size();

    return ensemble;
  }

} // namespace branch_and_bound
} // namespace bayes
