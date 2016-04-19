#include <limits>
#include <sstream>
#include "branch_and_bound.h"

namespace bayes {
namespace branch_and_bound {

BranchAndBound::BranchAndBound(const std::vector<Variable>& variables) {
  variables_ = variables;
  Initialize();
}

void BranchAndBound::Initialize() {
  Graph base(variables_);
  graphs_.push(base);
  upper_bound_ = base.score();
  lower_bound_ = std::numeric_limits<long double>::min();
}

Graph BranchAndBound::Run(long double target_gap) {
  unsigned evaluated = 0, discarded = 0;
  Graph best_graph;
  while (!graphs_.empty()) {
    Graph current_graph = graphs_.top();
    graphs_.pop();
    current_graph.ReadyForUse(variables_);
    std::cerr << "\rQueue size: " << graphs_.size() << " Evaluated: "
              << evaluated++ << " Discarded: " << discarded
              << " Best score: " << best_graph.score() << "        ";
    if (current_graph.score() > best_graph.score()) {
      std::vector<unsigned> cycle;
      current_graph.FindCycle(cycle);
      if (!cycle.empty()) {  // Not a DAG, gotta fix this.
        // std::cout << "Cycle: ";
        // SP(cycle);
        unsigned edge = 1;
        while (edge < cycle.size()) {
          unsigned parent = cycle[edge - 1], child = cycle[edge];
          Graph next_graph = current_graph;
          if (next_graph.RemoveArc(parent, child)) {
            next_graph.mutable_variables().clear();
            graphs_.push(next_graph);
          }
          current_graph.mutable_h_matrix()[parent][child] = ArcStatus::PRESENT;
          current_graph.mutable_h_matrix()[child][parent] =
              ArcStatus::PROHIBITED;
          edge++;
        }
        upper_bound_ = current_graph.score();
      } else {  // Now we certainly have a DAG
        best_graph = current_graph;
        lower_bound_ = best_graph.score();
      }
    } else {
      discarded++;
    }
  }
  return best_graph;
}

}  // namespce branch_and_bound
}  // namespace bayes
