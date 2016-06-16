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
  Grasp(std::vector<Variable> &pvar, InvertedIndex *pindex);

  // Runs Grasp and returns the best graph found.
  Graph run(int maxParents, int maxRuns);

 private:
  //
  std::vector<Variable> var;

  // The best graph found so far at run.
  Graph best;

  // The score of the best graph found.
  long double best_score;

  //
  InvertedIndex *index;
  
  void RunRound(int maxParents);
};

}  // namespace heuristic
}  // namespace branch_and_bound
}  // namespace bayes

#endif  //_L2RF_BAYES_BRANCH_AND_BOUND_HEURISTIC_GRASP_H_
