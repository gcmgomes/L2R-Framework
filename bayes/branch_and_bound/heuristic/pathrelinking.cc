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

  Graph bestGraph = origin;
  Graph current = origin;

  for (unsigned i = 0; i < origin.variables().size(); i++) {
    Bitset bOrigin = origin.variables()[i].parent_set();
    Bitset bDestiny = destiny.variables()[i].parent_set();

    for (unsigned j = 0; j < bOrigin.bit_count(); j++) {
      if (not bOrigin.at(j) && bDestiny.at(j)) {
        toBeAdded.push(std::make_pair(i, j));
      } else if (bOrigin.at(j) && not bDestiny.at(j)) {
        toBeRemoved.insert(std::make_pair(i, j));
      }
    }
  }

  std::vector<unsigned> cycleTmp;

  while (not toBeAdded.empty()) {
    std::pair<unsigned, unsigned> edge = toBeAdded.front();
    toBeAdded.pop();
    cycleTmp.clear();

    current.AddArc(edge.second, edge.first, this->index_);
    current.FindCycle(cycleTmp);
    while (not cycleTmp.empty()) {
      cycleTmp.push_back(*cycleTmp.begin());
      bool foundNewCycle = false;
      for (unsigned i = 1; i < cycleTmp.size(); i++) {
        unsigned parent = cycleTmp[i - 1] - 1;
        unsigned child = cycleTmp[i] - 1;
        if (toBeRemoved.find(std::make_pair(child, parent)) !=
            toBeRemoved.end()) {
          toBeRemoved.erase(std::make_pair(child, parent));
          current.RemoveArc(parent, child);
          cycleTmp.clear();
          foundNewCycle = true;
          current.FindCycle(cycleTmp);
          break;
        }
      }
      if (not foundNewCycle)
        cycleTmp.clear();
    }

    bayes::branch_and_bound::heuristics::GreedyLocalSearch gls(current);
    gls.Run(1000);

    if (bestGraph < current) {
      bestGraph = current;
    }
    if (bestGraph < gls.seed()) {
      bestGraph = gls.seed();
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
