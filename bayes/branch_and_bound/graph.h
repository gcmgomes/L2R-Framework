// This class is the final building block of the B&B algorithm. It embodies the
// notions of topology, prohibition matrix and structure score needed by the
// proceedure. Furthermore, it implements the required operations, such as cycle
// detection and the ordering function needed by the priority queue that will be
// used.

#ifndef _L2RF_BAYES_BRANCH_AND_BOUND_GRAPH_H_
#define _L2RF_BAYES_BRANCH_AND_BOUND_GRAPH_H_

#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>
#include "variable.h"

namespace bayes {
namespace branch_and_bound {

enum class ArcStatus {
  PRESENT = 1,     // Arc MUST exist in the structure
  PROHIBITED = 2,  // Arc CANNOT exist in the structure
  FREE = 3,        // Arc is free to be there or not
};

class Graph {
 public:
  Graph();

  Graph(const Graph& other);

  Graph(const std::vector<Variable>& variables);

  long double score() const {
    return score_;
  }

  const std::vector<Variable>& variables() const {
    return variables_;
  }

  std::vector<Variable>& mutable_variables() {
    return variables_;
  }

  const std::vector<std::vector<ArcStatus>>& h_matrix() const {
    return h_matrix_;
  }

  std::vector<std::vector<ArcStatus>>& mutable_h_matrix() {
    return h_matrix_;
  }

  // Finds a cycle in |this| and stores it in |cycle|. If none exist, the vector
  // will be empty.
  void FindCycle(std::vector<unsigned>& cycle) const;

  bool operator<(const Graph& other) const {
    return this->score() < other.score();
  }

  // Removes the arc |parent_variable| -> |child_variable| and updates the score
  // to reflect the change. Returns true if the removal was successful, false
  // otherwise.
  bool RemoveArc(unsigned parent_variable, unsigned child_variable);

  std::string ToString(std::string left_padding = "") const;

 private:

  // Initializes the required structures and values.
  void Initialize();

  long double score_;
  std::vector<Variable> variables_;
  std::vector<std::vector<ArcStatus>> h_matrix_;
};

}  // namespce branch_and_bound
}  // namespace bayes

#endif
