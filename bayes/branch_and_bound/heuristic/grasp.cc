#include <time.h>
#include <cstdlib>
#include <limits.h>
#include <algorithm>
#include "grasp.h"
#include "../cache.h"
#include "../external_priority_queue/heuristics/greedy_local_search.h"
#include "../external_priority_queue/heuristics/backtrack_local_search.h"

namespace bayes {
namespace branch_and_bound {
namespace heuristic {

Grasp::Grasp(const std::vector<Variable> &variables,
             const InvertedIndex *index) {
  this->variables_ = variables;
  this->index_ = index;
  srand(time(NULL));
}

Graph Grasp::Run(int maxParents, int maxRuns) {
  maxRuns = std::max(0, maxRuns);
  while (maxRuns--) {
    this->RunRound(maxParents);
    Graph result(this->variables_);
    
    
    bayes::branch_and_bound::heuristics::GreedyLocalSearch gls(result);
    gls.Run(1000);
    result = gls.seed();
    
    /*
    bayes::branch_and_bound::heuristics::BacktrackLocalSearch bls(result);
    bls.Run(1000);
    result = bls.seed();
    */
    
    if (result.score() > this->best_graph_.score()) {
      this->best_graph_ = result;
    }
  }
  
  return this->best_graph_;
}

void Grasp::RunRound(int maxParents) {
  maxParents = std::max(0, maxParents);

  for (unsigned i = 0; i < this->variables_.size(); i++) {
    this->variables_[i].mutable_parent_set().clear();
  }
  
  // Getting a random partial order...
  for (unsigned i = 0; i < this->variables_.size(); i++) {
    int randomPosition = (rand() % (this->variables_.size() - i)) + i;
    std::swap(this->variables_[i], this->variables_[randomPosition]);
  }

  while (maxParents--) {
    for (unsigned i = 1; i < this->variables_.size(); i++) {
      long double best_parent_score = LLONG_MIN;
      int best_parent = -1;
       
      // Checking which variable will increase the score the most.
      for (unsigned j = 0; j < i; j++) {
        if (this->variables_[i].parent_set().at(j)) {
          continue;
        }
        
        this->variables_[i].mutable_parent_set().Set(j, true);
        if (this->variables_[i].cache()->cache().find(
                this->variables_[i].parent_set()) ==
            this->variables_[i].cache()->cache().end()) {
          long double log_likelihood = this->variables_[i].LogLikelihood(
              *this->index_, this->variables_);
          
          const CacheEntry entry(
              log_likelihood,
              this->variables_[i].FreeParameters(this->variables_));
          
          Cache *cache = this->variables_[i].mutable_cache();
          cache->Insert(this->variables_[i].parent_set(), entry);
        }
        
        long double var_score = this->variables_[i].score();
        
        if (best_parent_score < var_score) {
          best_parent_score = var_score;
          best_parent = j;
        }
        this->variables_[i].mutable_parent_set().Set(j, false);
      }
      
      // If found a parent that increases the variables score...
      if (best_parent != -1) {
        this->variables_[i].mutable_parent_set().Set(best_parent, true);
      }
    }
  }
}

}  // namespace heuristic
}  // namespace branch_and_bound
}  // namespace bayes
