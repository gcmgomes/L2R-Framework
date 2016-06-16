// Theses structures represent the nodes of the bayesian network to be
// generated. Mostly, we are interested in the parent set of each variable,
// as they are at the core of the structure learning task.
// We employ a pointer to a Cache instead of a hard copoy due to efficiency
// and memory reasons.
#ifndef _L2RF_BAYES_BRANCH_AND_BOUND_VARIABLE_H_
#define _L2RF_BAYES_BRANCH_AND_BOUND_VARIABLE_H_

#include <cstdlib>
#include <iostream>
#include <memory>
#include <set>
#include <vector>
#include "bitset.h"
#include "cache.h"
#include "external_queue.h"
#include "inverted_index.h"

namespace bayes {
namespace branch_and_bound {

class Variable {
 public:
  Variable(unsigned variable_id, Cache* cache, ExternalQueue* external_queue,
           size_t variable_count);

  unsigned variable_id() const {
    return variable_id_;
  }

  const Bitset& parent_set() const {
    return parent_set_;
  }

  Bitset& mutable_parent_set() {
    return parent_set_;
  }

  const std::vector<unsigned>& categories() const {
    return categories_;
  }

  std::vector<unsigned>& mutable_categories() {
    return categories_;
  }

  const Cache* cache() const {
    return cache_;
  }

  Cache* mutable_cache() const {
    return cache_;
  }

  const ExternalQueue* external_queue() const {
    return external_queue_;
  }

  long double score() const;

  void BuildCache(const InvertedIndex& index,
                  const std::vector<Variable>& variables);

  long double LogLikelihood(const InvertedIndex& index,
                            const std::vector<Variable>& variables) const;

  unsigned long long FreeParameters(
      const std::vector<Variable>& variables) const;

  static void InitializeVariables(const InvertedIndex& index,
                                  std::vector<Variable>& variables,
                                  std::vector<ExternalQueue>& external_queues,
                                  std::vector<Cache>& caches);

  std::string ToString() const;

  // Sets |parent_set_| to the best entry found in |cache_|.
  void FindBestEntry();

 private:
  // Recursively determines a configuration for the parents of the variable.
  long double LLOuterSum(const InvertedIndex& index,
                         const std::vector<Variable>& variables,
                         const std::unordered_set<unsigned>& intersection,
                         std::vector<unsigned>& configuration) const;

  // Returns the innermost summation of the LogLikelihood metric.
  long double LLInnerSum(
      const InvertedIndex& index,
      const std::unordered_set<unsigned>& intersection) const;

  unsigned variable_id_;
  Bitset parent_set_;
  std::vector<unsigned> categories_;
  Cache* cache_;
  ExternalQueue* external_queue_;
};

}  // namespce branch_and_bound
}  // namespace bayes

#endif
