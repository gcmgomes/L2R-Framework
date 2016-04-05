// Theses structures represent the nodes of the bayesian network to be
// generated. Mostly, we are interested in the parent set of each variable,
// as they are at the core of the structure learning task.
#ifndef _L2RF_BAYES_BRANCH_AND_BOUND_CACHE_H_
#define _L2RF_BAYES_BRANCH_AND_BOUND_CACHE_H_

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>
#include "bitset.h"

namespace bayes {
namespace branch_and_bound {

class CacheEntry {
 public:
  CacheEntry();
  CacheEntry(long double log_likelihood, unsigned long long free_parameters);

  long double log_likelihood() const {
    return log_likelihood_;
  }

  unsigned long long free_parameters() const {
    return free_parameters_;
  }

  long double score(long double w) const {
    return log_likelihood_ - ((long double)free_parameters_) * w;
  }

  std::string ToString() const;

  std::string LogString() const;

 private:
  long double log_likelihood_;
  unsigned long long free_parameters_;
};

class Cache {
 public:
  // Defaults to AIC.
  Cache();

  Cache(long double w);

  void Insert(const Bitset& bitset, const CacheEntry& entry);

  long double w() const {
    return w_;
  }

  const std::unordered_map<Bitset, CacheEntry>& cache() const {
    return cache_;
  }

  const CacheEntry& at(const Bitset& bitset) const {
    return cache_.at(bitset);
  }

  bool IsConsistent() const;

  // Opens the file pointed by |file_path| to serve as |repository_| for this cache.
  void OpenRepository(const std::string& file_path);

  // If |repository_| is open, dumps the contents of |cache_| to |repository_|,
  // otherwise does nothing.
  void WriteToRepository();

 private:
  // This constant indicates the type of criateria we are using. At first, it
  // should either be set to 1 (AIC) or (log N) / 2 (MDL, N = # of instances).
  long double w_;

  std::unordered_map<Bitset, CacheEntry> cache_;

  // The file to hold the cache as it is built.
  std::fstream repository_;
};

}  // namespce branch_and_bound
}  // namespace bayes

#endif
