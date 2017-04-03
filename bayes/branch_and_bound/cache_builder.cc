#include "cache_builder.h"
#include <cmath>
#include <sstream>

namespace bayes {
namespace branch_and_bound {

CacheBuilder::CacheBuilder(Variable* variable, ExternalQueue* external_queue) {
  variable_ = variable;
  cache_ = variable_->mutable_cache();
  external_queue_ = external_queue;
}

static void AugmentSupersets(
    unsigned variable_id, long double w, const std::vector<Variable>& variables,
    std::unordered_map<Bitset, std::pair<unsigned, CacheEntry> >&
        best_subset_entries,
    Bitset& parent_set, ExternalQueue* external_queue_) {
  auto& subset_entry = best_subset_entries[parent_set].second;
  auto variable = variables.cbegin();
  while (variable != variables.cend()) {
    unsigned id = variable->variable_id();
    if (id != variable_id && !parent_set.at(id) &&
        variable->categories().size() > 1) {
      parent_set.Set(id, true);
      auto& entry = best_subset_entries[parent_set];
      if (!entry.first) {
        external_queue_->push(parent_set);
        entry.first = parent_set.high_bits().size();
        entry.second = subset_entry;
      } else if (entry.second.score(w) < subset_entry.score(w)) {
        entry.second = subset_entry;
      }
      best_subset_entries[parent_set].first--;
      parent_set.Set(id, false);
    }
    ++variable;
  }
}

void CacheBuilder::Build(const InvertedIndex& index,
                         const std::vector<Variable>& variables) {
  if (variable_->categories().size() < 2) {
    return;
  }
  std::cout << "Variable: " << variable_->variable_id() << std::endl;
  unsigned long long evaluated = 0, discarded = 0;
  long double w = cache_->w();
  Bitset& parent_set = variable_->mutable_parent_set() =
      Bitset(variables.size());
  std::unordered_map<Bitset, std::pair<unsigned, CacheEntry> >
      best_subset_entries;
  external_queue_->push(parent_set);
  CacheEntry best_entry;
  while (!external_queue_->empty()) {
    parent_set = external_queue_->front();
    external_queue_->pop();
    std::cout << "\rQueue size: " << external_queue_->size()
              << " Evaluated: " << evaluated++ << " Discarded: " << discarded
              << " Cache size: " << cache_->cache().size()
              << " Hash_size: " << best_subset_entries.size()
              << " Set_size: " << parent_set.high_bits().size()
              << "                ";
    bool is_empty_set = parent_set.high_bits().empty();
    unsigned long long free_parameters = variable_->FreeParameters(variables);
    auto& subset_entry = best_subset_entries[parent_set].second;
    bool missing_edges = best_subset_entries[parent_set].first;
    if ((is_empty_set || free_parameters + subset_entry.score(w) <= 0) &&
        !missing_edges) {  // Passed Lemma 2
      long double log_likelihood = variable_->LogLikelihood(index, variables);
      CacheEntry entry(log_likelihood, free_parameters);
      if (is_empty_set) {
        best_subset_entries[parent_set].second = entry;
      }
      if (is_empty_set ||
          subset_entry.score(w) <= entry.score(w)) {  // Passed Lemma 1
        subset_entry = entry;
        cache_->Insert(parent_set, entry);
        if (best_entry.score(w) < entry.score(w)) {
          best_entry = entry;
        }
      } else {
        discarded++;
      }
      // Even if Lemma 1 doesn't pass, we need to check this set's
      // supersets.
      AugmentSupersets(variable_->variable_id(), w, variables,
                       best_subset_entries, parent_set, external_queue_);
    } else {
      discarded++;
    }
    best_subset_entries.erase(parent_set);
  }
  std::cout << std::endl;
  std::cout.flush();
  cache_->Flush();
}

}  // namespce branch_and_bound
}  // namespace bayes
