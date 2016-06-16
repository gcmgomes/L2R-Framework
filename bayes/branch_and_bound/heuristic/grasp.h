#ifndef _L2RF_BAYES_BRANCH_AND_BOUND_HEURISTIC_GRASP_H_
#define _L2RF_BAYES_BRANCH_AND_BOUND_HEURISTIC_GRASP_H_

#include <vector>

#include "../graph.h"
#include "../variable.h"
#include "../inverted_index.h"

namespace bayes {
namespace branch_and_bound {
namespace heuristic {

class Grasp {
 public:
  Grasp(const std::vector<Variable> &variables, const InvertedIndex *index);

  // Runs Grasp and returns the best graph found.
  // Received the maximum number of parents allowed for each variable,
  // and the number of runs executed.
  Graph Run(int maxParents, int maxRuns);

 private:
  // Method that runs one iteration of the heuristic.
  void RunRound(int maxParents);

  std::vector<Variable> variables_;

  // The best graph found by Run.
  Graph best_graph_;

  const InvertedIndex *index_;
};

}  // namespace heuristic
}  // namespace branch_and_bound
}  // namespace bayes

#endif  //_L2RF_BAYES_BRANCH_AND_BOUND_HEURISTIC_GRASP_H_
