// The final piece of the any-time ExternalBranch and Bound algorithm that
// discovers the
// optimal topology for a Bayesian Network for a given dataset.
#ifndef _L2RF_BAYES_BRANCH_AND_BOUND_EXTERNAL_BRANCH_AND_BOUND_H_
#define _L2RF_BAYES_BRANCH_AND_BOUND_EXTERNAL_BRANCH_AND_BOUND_H_

#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>
#include "graph_external_priority_queue.h"

namespace bayes {
namespace branch_and_bound {

class ExternalBranchAndBound {
 public:
  ExternalBranchAndBound(unsigned bottom_frequency,
                         const std::vector<Variable>& variables,
                         GraphExternalPriorityQueue* graphs);

  ExternalBranchAndBound(const std::vector<Variable>& variables);

  // Executes the B&B algorithm. When the gap gets smaller than |target_gap|,
  // returns the optimal structure found so far.
  Graph Run(long double target_gap);

 private:
  // Makes everything ready to run the B&B algorithm.
  void Initialize();

  unsigned bottom_frequency_;

  std::vector<Variable> variables_;
  std::unique_ptr<GraphExternalPriorityQueue> graphs_;

  long double upper_bound_;
  long double lower_bound_;
};

}  // namespce branch_and_bound
}  // namespace bayes

#endif
