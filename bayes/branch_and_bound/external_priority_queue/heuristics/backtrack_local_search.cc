#include <limits>
#include <sstream>
#include "backtrack_local_search.h"

namespace bayes {
namespace branch_and_bound {
namespace heuristics {

BacktrackLocalSearch::BacktrackLocalSearch(const bayes::branch_and_bound::Graph& seed)
    : seed_(seed) {
}

static void Powerset(std::vector<std::set<unsigned>>& powerset, unsigned id, unsigned n, unsigned max_rounds, std::set<unsigned>& parent) {
  while(id < n && parent.size() < max_rounds) {
    parent.insert(id);
    powerset.push_back(parent);
    Powerset(powerset, id+1, n, max_rounds, parent);
    parent.erase(id);
    id++;
  }
}

bool BacktrackLocalSearch::Run(unsigned max_rounds) {
  bool augmented = false;
  bayes::branch_and_bound::Graph best_graph = seed_;
  std::vector<std::pair<unsigned, bayes::branch_and_bound::Bitset>> best_parent_sets;
  if (Augment(best_parent_sets)) {
    augmented = true;
    std::vector<std::set<unsigned>> powerset;
    std::set<unsigned> dummy;
    std::cerr << std::endl << "Computing powerset" << std::endl;
    Powerset(powerset, 0, best_parent_sets.size(), max_rounds, dummy);
    std::cerr << " Done" << std::endl;
    unsigned subset = 0;
    while(subset < powerset.size()) {
      bayes::branch_and_bound::Graph current_graph = seed_;
      auto parent_set = powerset[subset].begin();
      while(parent_set != powerset[subset].end()) {
        unsigned variable_id = best_parent_sets[*parent_set].first;
        auto bitset = seed_.mutable_variables()[variable_id].mutable_parent_set();
        current_graph.mutable_variables()[variable_id].mutable_parent_set() = best_parent_sets[*parent_set].second;
        ++parent_set;
      }
      current_graph.RecomputeScore();
      std::vector<unsigned> cycle;
      current_graph.FindCycle(cycle);
      if(cycle.empty() && current_graph.score() > best_graph.score()) {
        best_graph = current_graph;
      }
      subset++;
    }
  }
  seed_ = best_graph;
  return augmented;
}

long double BacktrackLocalSearch::FindBestAugmentingEntry(
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

bool BacktrackLocalSearch::Augment(
  std::vector<std::pair<unsigned, bayes::branch_and_bound::Bitset>>& best_parent_sets) {
  unsigned variable_id = 0;
  bool augmented = false;
  bayes::branch_and_bound::Bitset best_parent_set(seed_.variables().size());
  while (variable_id < seed_.variables().size()) {
    long double original_score = seed_.variables().at(variable_id).score();
    bayes::branch_and_bound::Bitset parent_set(seed_.variables().size());
    long double next_score = FindBestAugmentingEntry(variable_id, parent_set);
    if ((next_score - original_score) > 0.0) {
      augmented = true;
      best_parent_sets.push_back(std::make_pair(variable_id, parent_set));
    }
    ++variable_id;
  }
  return augmented;
}

}  // namespace heuristics
}  // namespce branch_and_bound
}  // namespace bayes
