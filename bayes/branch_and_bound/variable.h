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
#include "instance.h"

namespace bayes {
namespace branch_and_bound {

enum class Criterion {
  MINIMUM_DESCRIPTION_LEGNTH,    // w = (log N) / 2
  AKAIKE_INFORMATION_CRITERION,  // w = 1.
};

class Variable {
 public:
  Variable(unsigned variable_id, Cache* cache);

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

  long double score() const;

  void BuildCache(const std::vector<Instance>& instances,
                  const std::vector<Variable>& variables);

  long double LogLikelihood(const std::vector<Instance>& instances,
                            const std::vector<Variable>& variables) const;

  unsigned long long FreeParameters(
      const std::vector<Variable>& variables) const;

  static void InitializeVariables(
      const std::vector<Instance>& instances, std::vector<Variable>& variables,
      std::vector<Cache>& caches,
      Criterion criterion = Criterion::AKAIKE_INFORMATION_CRITERION);

  std::string ToString() const;

 private:
  // Recursively determines a configuration for the parents of the variable.
  long double LLOuterSum(const std::vector<Instance>& instances,
                         const std::vector<Variable>& variables,
                         std::vector<unsigned>& configuration) const;

  // Returns the innermost summation of the LogLikelihood metric.
  long double LLInnerSum(const std::vector<Instance>& instances,
                         std::vector<unsigned>& configuration) const;

  unsigned variable_id_;
  Bitset parent_set_;
  std::vector<unsigned> categories_;
  Cache* cache_;
};

}  // namespce branch_and_bound
}  // namespace bayes

#endif
