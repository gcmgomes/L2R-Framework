#include "grasp.h"
#include "../cache.h"
#include "../external_priority_queue/heuristics/backtrack_local_search.h"
#include "../external_priority_queue/heuristics/greedy_local_search.h"
#include <algorithm>
#include <cstdlib>
#include <limits.h>
#include <time.h>

namespace bayes {
namespace branch_and_bound {
namespace heuristic {

Grasp::Grasp(const std::vector<Variable> &variables,
             const InvertedIndex *index) {
  this->variables_ = variables;
  this->index_ = index;
}

void Grasp::run(int maxParents, int maxRuns) {
  srand(time(NULL));
  this->solutions_.clear();
  maxRuns = std::max(0, maxRuns);
  while (maxRuns--) {
    this->runRound(maxParents);
    Graph result(this->variables_);

    bayes::branch_and_bound::heuristics::GreedyLocalSearch gls(result);
    gls.Run(50);
    result = gls.seed();

    /*
    bayes::branch_and_bound::heuristics::BacktrackLocalSearch bls(result);
    bls.Run(1000);
    result = bls.seed();
    */

    this->solutions_.push_back(result);
    if (result.score() > this->best_graph_.score()) {
      this->best_graph_ = result;
    }
  }

  sort(this->solutions_.begin(), this->solutions_.end());
  reverse(this->solutions_.begin(), this->solutions_.end());
}

// Returns the best graph generated by the last run of grasp.
Graph Grasp::getBest() {
  return *this->solutions_.begin();
}

// Returns all solutions generated by the last run of grasp.
std::vector<Graph> Grasp::getAll() {
  return this->solutions_;
}

void Grasp::runRound(int maxParents) {
  maxParents = std::max(0, maxParents);

  std::vector<unsigned> order(this->variables_.size(), 0);
  for (unsigned i = 0; i < this->variables_.size(); i++) {
    this->variables_[i].mutable_parent_set().clear();
    order[i] = i;
  }

  // Getting a random partial order...
  for (unsigned i = 0; i < this->variables_.size(); i++) {
    int randomPosition = (rand() % (this->variables_.size() - i)) + i;
    std::swap(order[i], order[randomPosition]);
  }

  while (maxParents--) {
    for (unsigned i = 1; i < this->variables_.size(); i++) {
      int child_id = order[i];
      int parent_id = 0;
      long double best_parent_score = LLONG_MIN;
      int best_parent = -1;

      // Checking which variable will increase the score the most.
      for (unsigned j = 0; j < i; j++) {
        parent_id = order[j];
        if (this->variables_[child_id].parent_set().at(parent_id)) {
          continue;
        }

        this->variables_[child_id].mutable_parent_set().Set(parent_id, true);
        if (this->variables_[child_id].cache()->cache().find(
                this->variables_[child_id].parent_set()) ==
            this->variables_[child_id].cache()->cache().end()) {
          long double log_likelihood = this->variables_[child_id].LogLikelihood(
              *this->index_, this->variables_);

          const CacheEntry entry(
              log_likelihood,
              this->variables_[child_id].FreeParameters(this->variables_));

          Cache *cache = this->variables_[child_id].mutable_cache();
          cache->Insert(this->variables_[child_id].parent_set(), entry);
        }

        long double var_score = this->variables_[child_id].score();

        if (best_parent_score < var_score) {
          best_parent_score = var_score;
          best_parent = parent_id;
        }
        this->variables_[child_id].mutable_parent_set().Set(parent_id, false);
      }

      // If found a parent that increases the variables score...
      if (best_parent != -1) {
        this->variables_[child_id].mutable_parent_set().Set(best_parent, true);
      }
    }
  }
}

}  // namespace heuristic
}  // namespace branch_and_bound
}  // namespace bayes
