#ifndef _L2RF_BAYES_BRANCH_AND_BOUND_HEURISTIC_PATHRELINKING_H_
#define _L2RF_BAYES_BRANCH_AND_BOUND_HEURISTIC_PATHRELINKING_H_

#include <vector>

#include "../graph.h"
#include "../inverted_index.h"
#include "../variable.h"

namespace bayes {
namespace branch_and_bound {
namespace heuristic {

class PathRelinking {
 public:
  // TODO: Receive a Scorer here as well.
  PathRelinking(const std::vector<Variable> &variables,
                const InvertedIndex *index);

  // TODO: Think in nice parameters for the path relinking method.
  // Ideas:
  //    * Number of paths evaluated.
  Graph run();

 private:
  std::vector<Variable> variables_;

  const InvertedIndex *index_;

  //Scorer scorer_;

  std::vector<Graph> baseSolutions_;

  // This method generates the first solutions used by
  // the path-relinking method.
  void generateBaseSolutions();

  // This method performs the walk from graph origin to destiny.
  // And returns the best graph found.
  Graph walk(Graph &origin, Graph &destiny);
};

}  // namespace heuristic
}  // namespace branch_and_bound
}  // namespace bayes

#endif  //_L2RF_BAYES_BRANCH_AND_BOUND_HEURISTIC_PATHRELINKING_H_
