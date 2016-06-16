#include <time.h>
#include <cstdlib>
#include <limits.h>
#include <algorithm>
#include "grasp.h"
#include "../cache.h"

namespace bayes {
namespace branch_and_bound {
namespace heuristic {

Grasp::Grasp(std::vector<Variable> &pvar, InvertedIndex *pindex) {
  this->var = pvar;
  this->best_score = LLONG_MIN;
  this->index = pindex;
  srand(time(NULL));
}

Graph Grasp::run(int maxParents, int maxRuns) {
  maxRuns = std::max(0, maxRuns);
  while (maxRuns--) {
    this->RunRound(maxParents);
    Graph result(this->var);
    if (result.score() > this->best.score()) {
      this->best = result;
    }
  }

  return this->best;
}

void Grasp::RunRound(int maxParents) {
  maxParents = std::max(0, maxParents);
  
  for (unsigned i = 0; i < this->var.size(); i++) {
    this->var[i].mutable_parent_set().clear();
  }
  
  // Getting a random partial order...
  for (unsigned i = 0; i < this->var.size(); i++) {
    int randomPosition = (rand() % (this->var.size() - i)) + i;
    std::swap(this->var[i], this->var[randomPosition]);
  }
  
  while (maxParents--) {
    for (unsigned i = 1; i < this->var.size(); i++) {
      long double best_parent_score = LLONG_MIN;
      int best_parent = -1;
      
      // Checking which variable will increase the score the most.
      for (unsigned j = 0; j < i; j++) {
        if (this->var[i].parent_set().at(j)) {
          continue;
        }
        
        this->var[i].mutable_parent_set().Set(j, true);
        if (var[i].cache()->cache().find(var[i].parent_set()) ==
            var[i].cache()->cache().end()) {
          long double log_likelihood =
              this->var[i].LogLikelihood(*this->index, this->var);

          const CacheEntry entry(log_likelihood,
                                 this->var[i].FreeParameters(this->var));

          Cache *cache = this->var[i].mutable_cache();
          cache->Insert(this->var[i].parent_set(), entry);
        }

        long double var_score = this->var[i].score();

        if (best_parent_score < var_score) {
          best_parent_score = var_score;
          best_parent = j;
        }
        this->var[i].mutable_parent_set().Set(j, false);
      }
      
      // If found a parent that increases the variables score...
      if (best_parent != -1) {
        this->var[i].mutable_parent_set().Set(best_parent, true);
      }
    }
  }
}

}  // namespace heuristic
}  // namespace branch_and_bound
}  // namespace bayes
