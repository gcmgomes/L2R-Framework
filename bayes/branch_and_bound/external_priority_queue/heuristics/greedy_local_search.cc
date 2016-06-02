#include <limits>
#include <sstream>
#include "greedy_local_search.h"

namespace bayes {
namespace branch_and_bound {
namespace heuristics {

GreedyLocalSearch::GreedyLocalSearch(const bayes::branch_and_bound::Graph& seed)
    : seed_(seed) {
}

bool GreedyLocalSearch::Run(unsigned max_rounds) {
  bool augmented = false;
  while (max_rounds && Augment()) {
    augmented = true;
    max_rounds--;
  }
  return augmented;
}

long double GreedyLocalSearch::FindBestAugmentingEntry(
    unsigned variable_id, bayes::branch_and_bound::Bitset& best_parent_set) {
  auto& variable = seed_.mutable_variables()[variable_id];
  long double current_score = variable.score();
  bayes::branch_and_bound::Bitset original_parent_set = variable.parent_set();
  auto entry = variable.cache()->cache().cbegin();
  while (entry != variable.cache()->cache().cend()) {
    long double new_score = entry->second.score(variable.cache()->w());
    if (current_score < new_score) {
      variable.mutable_parent_set() = entry->first;
      std::vector<unsigned> cycle;
      seed_.FindCycle(cycle);
      if (cycle.empty()) {
        best_parent_set = entry->first;
        current_score = new_score;
      }
      variable.mutable_parent_set() = original_parent_set;
    }
    ++entry;
  }
  return current_score;
}

bool GreedyLocalSearch::Augment() {
  unsigned variable_id = 0;
  unsigned best_variable_id = 0;
  long double best_score_delta = 0;
  bool augmented = false;
  bayes::branch_and_bound::Bitset best_parent_set(seed_.variables().size());
  while (variable_id < seed_.variables().size()) {
    long double original_score = seed_.variables().at(variable_id).score();
    bayes::branch_and_bound::Bitset parent_set(seed_.variables().size());
    long double next_score = FindBestAugmentingEntry(variable_id, parent_set);
    if ((next_score - original_score) > best_score_delta) {
      augmented = true;
      best_variable_id = variable_id;
      best_parent_set = parent_set;
      best_score_delta = next_score - original_score;
    }
    ++variable_id;
  }
  if (augmented) {
    seed_.mutable_variables()[best_variable_id].mutable_parent_set() =
        best_parent_set;
    seed_.RecomputeScore();
  }
  return augmented;
}

}  // namespace heuristics
}  // namespce branch_and_bound
}  // namespace bayes
