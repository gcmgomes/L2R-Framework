#include <limits>
#include <sstream>
#include "external_branch_and_bound.h"

namespace bayes {
namespace branch_and_bound {

ExternalBranchAndBound::ExternalBranchAndBound(
    unsigned bottom_frequency, const std::vector<Variable>& variables,
    ReferenceExternalPriorityQueue* graphs) {
  bottom_frequency_ = bottom_frequency;
  variables_ = variables;
  graphs_.reset(graphs);
  Initialize();
}

ExternalBranchAndBound::ExternalBranchAndBound(
    const std::vector<Variable>& variables)
    : ExternalBranchAndBound(
          2, variables, new ReferenceExternalPriorityQueue(10000, 290000)) {
}

void ExternalBranchAndBound::Initialize() {
  Graph base(variables_);
  base.ReadyForUse(variables_);
  graphs_->push(base);
  upper_bound_ = base.score();
  lower_bound_ = std::numeric_limits<long double>::min();
  graphs_->mutable_bounding_score() = lower_bound_;
}

Graph ExternalBranchAndBound::Run(long double target_gap) {
  unsigned bottom = bottom_frequency_;
  unsigned evaluated = 0, discarded = 0, dags = 0;
  std::cerr << "Root score: " << graphs_->max().score() << std::endl;
  Graph best_graph;
  while (!graphs_->empty()) {
    bool newline = false;
    bottom--;
    Graph current_graph;
    if (bottom) {
      current_graph = graphs_->max();
      upper_bound_ = current_graph.score();
      graphs_->pop_max();
    } else {
      bottom = bottom_frequency_;
      current_graph = graphs_->min();
      graphs_->pop_min();
    }
    if (current_graph.score() > best_graph.score()) {
      std::vector<unsigned> cycle;
      current_graph.FindCycle(cycle);
      if (!cycle.empty()) {  // Not a DAG, gotta fix this.
        unsigned edge = 1;
        while (edge < cycle.size()) {
          unsigned parent = cycle[edge - 1], child = cycle[edge];
          Graph next_graph = current_graph;
          if (next_graph.RemoveArc(parent, child)) {
            std::vector<unsigned> next_cycle;
            next_graph.FindCycle(next_cycle);
            if (next_cycle.empty() && next_graph.score() > best_graph.score()) {
              newline = true;
              dags++;
              best_graph = next_graph;
              lower_bound_ = best_graph.score();
              graphs_->mutable_bounding_score() = best_graph.score();
              heuristics::GreedyLocalSearch gls(best_graph);
              if (gls.Run(10)) {
                std::cerr << std::endl
                          << "Augmentated DAG" << std::endl;
                best_graph = gls.seed();
              }
              lower_bound_ = best_graph.score();
              graphs_->mutable_bounding_score() = best_graph.score();
            } else if (!next_cycle.empty()) {
              graphs_->push(next_graph);
            }
          }
          current_graph.mutable_h_matrix()[parent][child] = ArcStatus::PRESENT;
          current_graph.mutable_h_matrix()[child][parent] =
              ArcStatus::PROHIBITED;
          edge++;
        }
      } else {  // Now we certainly have a DAG
        dags++;
        newline = true;
        best_graph = current_graph;
        lower_bound_ = best_graph.score();
        graphs_->mutable_bounding_score() = best_graph.score();
      }
    } else {
      discarded++;
    }
    std::cerr << "\rQueue size: " << graphs_->total_size()
              << " Evaluated: " << evaluated++ << " Discarded: " << discarded
              << " DAGs: " << dags << " Best score: " << best_graph.score()
              << " Gap: "
              << -((upper_bound_ - lower_bound_) / upper_bound_) * 100 << "\%"
              << "          ";
    // std::cout << evaluated << " " << graphs_->total_size() << " " <<
    // graphs_->memory_size() << " " << graphs_->external_size();
    if (newline) {
      std::cout << best_graph.ToString() << std::endl;
      std::cerr << std::endl;
    }
  }
  std::cerr << std::endl;
  return best_graph;
}

}  // namespce branch_and_bound
}  // namespace bayes
