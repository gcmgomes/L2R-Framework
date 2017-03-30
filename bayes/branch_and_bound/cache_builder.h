#ifndef _L2RF_BAYES_BRANCH_AND_BOUND_CACHE_BUILDER_H_
#define _L2RF_BAYES_BRANCH_AND_BOUND_CACHE_BUILDER_H_

#include "bitset.h"
#include "cache.h"
#include "external_queue.h"
#include "inverted_index.h"
#include "variable.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <set>
#include <vector>

namespace bayes {
namespace branch_and_bound {

class CacheBuilder {
 public:
  CacheBuilder(Variable* variable, ExternalQueue* external_queue);

  void Build(const InvertedIndex& index,
             const std::vector<Variable>& variables);

 private:
  Variable* variable_;
  Cache* cache_;
  ExternalQueue* external_queue_;
  inference::CPTable* cp_table_;
};

}  // namespace branch_and_bound
}  // namespace bayes

#endif
