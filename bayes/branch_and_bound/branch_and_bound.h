// The final piece of the any-time Branch and Bound algorithm that discovers the
// optimal topology for a Bayesian Network for a given dataset.
#ifndef _L2RF_BAYES_BRANCH_AND_BOUND_BRANCH_AND_BOUND_H_
#define _L2RF_BAYES_BRANCH_AND_BOUND_BRANCH_AND_BOUND_H_

#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>
#include "graph.h"
#include "heap.h"

namespace bayes {
namespace branch_and_bound {

class BranchAndBound {
 public:
  BranchAndBound(const std::vector<Variable>& variables);

  // Executes the B&B algorithm. When the gap gets smaller than |target_gap|,
  // returns the optimal structure found so far.
  Graph Run(long double target_gap);

 private:
  // Makes everything ready to run the B&B algorithm.
  void Initialize();

  std::vector<Variable> variables_;
  std::multiset<Graph> graphs_;

  long double upper_bound_;
  long double lower_bound_;
};

}  // namespce branch_and_bound
}  // namespace bayes

#endif
