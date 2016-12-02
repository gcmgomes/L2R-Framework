#include <queue>
#include <set>
#include <utility>

#include "../external_priority_queue/heuristics/greedy_local_search.h"
#include "grasp.h"
#include "pathrelinking.h"

namespace bayes {
namespace branch_and_bound {
namespace heuristic {

PathRelinking::PathRelinking(const std::vector<Variable> &variables,
                             const InvertedIndex *index) {
  // this->scorer_ = scorer;
  this->variables_ = variables;
  this->index_ = index;
}

// TODO: Think in nice parameters for the path relinking method.
Graph PathRelinking::run() {
  generateBaseSolutions();

  Graph bestGraph = *this->baseSolutions_.begin();

  for (int i = this->baseSolutions_.size() - 1; i >= 0; i--) {
    Graph result = walk(this->baseSolutions_[i], bestGraph);
    if (bestGraph < result) {
      bestGraph = result;
    }
  }

  return bestGraph;
}

// This method performs the walk from graph origin to destiny.
// And returns the best graph found.
Graph PathRelinking::walk(Graph &origin, Graph &destiny) {
  // Pseudo-code:
  //
  //  while(current != final) {
  //    --> Add an Edge
  //    while(hascycle(current)) {
  //      --> Remove first possible edge
  //          (An edge that is not in the final graph)
  //    }
  //    evaluate(current);
  //  }
  //
  //

  // Edges to be added, they're initially at destiny graph and not in origin.
  std::queue<std::pair<unsigned, unsigned> > toBeAdded;

  // Edges to be removed, they're initially at origin and not in destiny.
  std::set<std::pair<unsigned, unsigned> > toBeRemoved;

  // Vector used to check if a graph has cycles.
  std::vector<unsigned> cycleTmp;

  Graph bestGraph = origin;
  Graph current = origin;

  // If there is a cycle either at origin or destiny,
  // just return an empty graph.
  origin.FindCycle(cycleTmp);
  if (cycleTmp.size()) {
    return Graph();
  }

  destiny.FindCycle(cycleTmp);
  if (cycleTmp.size()) {
    return Graph();
  }

  // Discovering which edges will be added and which will be removed.
  for (unsigned i = 0; i < origin.variables().size(); i++) {
    const Bitset &bOrigin = origin.variables()[i].parent_set();
    const Bitset &bDestiny = destiny.variables()[i].parent_set();
    unsigned sz = std::max(bOrigin.bit_count(), bDestiny.bit_count());

    for (unsigned j = 0; j <= sz; j++) {
      if (not bOrigin.at(j) && bDestiny.at(j)) {
        // An edge will be added to the origin graph if it's not in the origin
        // graph and it's at the destiny.
        toBeAdded.push(std::make_pair(i, j));
      } else if (bOrigin.at(j) && not bDestiny.at(j)) {
        // Same logic as above, but the inverse.
        toBeRemoved.insert(std::make_pair(i, j));
      }
    }
  }

  // Adding the edges to origin.
  while (not toBeAdded.empty()) {
    std::pair<unsigned, unsigned> edge = toBeAdded.front();
    toBeAdded.pop();
    if (edge.first == edge.second)
      continue;
    cycleTmp.clear();

    // If adding the current edge creates a cycle, I'll remove edges until the
    // cycle is removed...
    current.AddArc(edge.second, edge.first, this->index_);
    current.FindCycle(cycleTmp);
    while (not cycleTmp.empty() && not toBeRemoved.empty()) {
      for (unsigned i = 1; i < cycleTmp.size(); i++) {
        unsigned parent = cycleTmp[i - 1];
        unsigned child = cycleTmp[i];
        if (toBeRemoved.find(std::make_pair(child, parent)) !=
            toBeRemoved.end()) {
          toBeRemoved.erase(std::make_pair(child, parent));
          current.RemoveArc(parent, child);
          break;
        }
      }

      current.FindCycle(cycleTmp);
    }

    if (cycleTmp.empty()) {
      if (bestGraph < current) {
        bestGraph = current;
      }

      bayes::branch_and_bound::heuristics::GreedyLocalSearch gls(current);
      gls.Run(10);
      if (bestGraph < gls.seed()) {
        bestGraph = gls.seed();
      }
    }
  }

  return bestGraph;
}

void PathRelinking::generateBaseSolutions() {
  Grasp grasp(this->variables_, this->index_);

  grasp.run(3, 200);
  this->baseSolutions_ = grasp.getAll();
}

}  // namespace heuristic
}  // namespace branch_and_bound
}  // namespace bayes
