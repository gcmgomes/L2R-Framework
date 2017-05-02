// Theses structures represent the nodes of the bayesian network to be
// generated. Mostly, we are interested in the parent set of each variable,
// as they are at the core of the structure learning task.
#ifndef _L2RF_BAYES_BRANCH_AND_BOUND_CACHE_H_
#define _L2RF_BAYES_BRANCH_AND_BOUND_CACHE_H_

#include "bitset.h"
#include "instance.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

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

enum class Criterion {
  MINIMUM_DESCRIPTION_LEGNTH,    // w = (log N) / 2
  AKAIKE_INFORMATION_CRITERION,  // w = 1.
};

class Cache {
 public:
  // Defaults to AIC.
  Cache();

  Cache(long double w, size_t maximum_cache_size);

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

  // Every bit set to 1 in |prohibited_bits| indicates that the given feature
  // cannot exist in that parent set. Every bit set to 1 in |mandatory_bits|
  // indicates that the given feature must exist in the set. Returns true if a
  // complying entry was found, false otherwise. Check the return value before
  // using the value stored in |resulting_bits|.
  bool BestCompliantEntry(const Bitset& prohibited_bits,
                          const Bitset& mandatory_bits,
                          Bitset& resulting_bits) const;

  // Opens the file pointed by |file_path| to serve as |repository_| for this
  // cache. Set |open_mode| appropriately!
  void OpenRepository(const std::string& file_path,
                      std::ios_base::openmode open_mode);
  
  void CloseRepository();

  void Flush();

  static void InitializeCaches(
      const std::string& directory_root_path,
      const std::vector<Instance>& instances, std::vector<Cache>& caches,
      size_t maximum_cache_size,
      Criterion criterion = Criterion::AKAIKE_INFORMATION_CRITERION);

  static void LoadCaches(const std::string& directory_root_path,
                         unsigned variable_count, std::vector<Cache>& caches);

  std::string ToString() const;

 private:
  // If |repository_| is open, dumps the contents of |cache_| to |repository_|,
  // otherwise does nothing.
  void WriteToRepository();

  // Reads every entry from |repository_| and stores it in |cache_|.
  void LoadFromRepository();

  // This constant indicates the type of criateria we are using. At first, it
  // should either be set to 1 (AIC) or (log N) / 2 (MDL, N = # of instances).
  long double w_;

  size_t maximum_cache_size_;

  std::unordered_map<Bitset, CacheEntry> cache_;

  // The file to hold the cache as it is built.
  std::fstream repository_;
};

}  // namespce branch_and_bound
}  // namespace bayes

#endif
