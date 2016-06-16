#include "variable.h"
#include <cassert>
#include <cmath>
#include <queue>
#include <sstream>
#include <unordered_map>

namespace bayes {
namespace branch_and_bound {

Variable::Variable(unsigned variable_id, Cache* cache,
                   ExternalQueue* external_queue, size_t variable_count) {
  variable_id_ = variable_id;
  cache_ = cache;
  external_queue_ = external_queue;
  parent_set_ = Bitset(variable_count);
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

long double Variable::score() const {
  if (cache_->cache().empty()) {
    return 0;
  }
  return cache_->at(parent_set_).score(cache_->w());
}

void Variable::BuildCache(const InvertedIndex& index,
                          const std::vector<Variable>& variables) {
  if (categories_.size() < 2) {
    return;
  }
  std::cout << "Variable: " << variable_id_ << std::endl;
  unsigned long long evaluated = 0, discarded = 0;
  assert(external_queue_ != NULL);
  assert(cache_ != NULL);
  long double w = cache_->w();
  parent_set_ = Bitset(variables.size());
  std::unordered_map<Bitset, std::pair<unsigned, CacheEntry> >
      best_subset_entries;
  external_queue_->push(parent_set_);
  CacheEntry best_entry;
  while (!external_queue_->empty()) {
    parent_set_ = external_queue_->front();
    external_queue_->pop();
    std::cout << "\rQueue size: " << external_queue_->size()
              << " Evaluated: " << evaluated++ << " Discarded: " << discarded
              << " Cache size: " << cache_->cache().size()
              << " Hash_size: " << best_subset_entries.size()
              << " Set_size: " << parent_set_.high_bits().size()
              << "                ";
    bool is_empty_set = parent_set_.high_bits().empty();
    unsigned long long free_parameters = FreeParameters(variables);
    auto& subset_entry = best_subset_entries[parent_set_].second;
    bool missing_edges = best_subset_entries[parent_set_].first;
    if ((is_empty_set || free_parameters + subset_entry.score(w) <= 0) &&
        !missing_edges) {  // Passed Lemma 2
      long double log_likelihood = LogLikelihood(index, variables);
      CacheEntry entry(log_likelihood, free_parameters);
      if (is_empty_set) {
        best_subset_entries[parent_set_].second = entry;
      }
      if (is_empty_set ||
          subset_entry.score(w) <= entry.score(w)) {  // Passed Lemma 1
        subset_entry = entry;
        cache_->Insert(parent_set_, entry);
        if (best_entry.score(w) < entry.score(w)) {
          best_entry = entry;
        }
      } else {
        discarded++;
      }
      // Even if Lemma 1 doesn't pass, we need to check this set's
      // supersets.
      AugmentSupersets(variable_id_, w, variables, best_subset_entries,
                       parent_set_, external_queue_);
    } else {
      discarded++;
    }
    best_subset_entries.erase(parent_set_);
  }
  std::cout << std::endl;

  if (false) {
    std::cerr << std::endl
              << "##########################################" << std::endl;
    std::cerr << "Final cache for variable " << variable_id_ << "has "
              << cache_->cache().size() << " entries: " << std::endl;
    auto it = cache_->cache().cbegin();
    while (it != cache_->cache().cend()) {
      std::cerr << it->first.ToString() << ": " << it->second.ToString()
                << " Score: " << it->second.score(w) << std::endl;
      ++it;
    }
    std::cerr << "Best local config:" << std::endl;
    std::cerr << parent_set_.ToString() << ": " << best_entry.ToString()
              << " Score: " << best_entry.score(w) << std::endl;
    std::cerr << "Final cache for variable " << variable_id_ << "has "
              << cache_->cache().size() << " entries: " << std::endl;
    std::cerr << "##########################################" << std::endl;
  }
  cache_->Flush();
}

long double Variable::LogLikelihood(
    const InvertedIndex& index, const std::vector<Variable>& variables) const {
  std::vector<unsigned> configuration;
  std::unordered_set<unsigned> intersection;
  return LLOuterSum(index, variables, intersection, configuration);
}

unsigned long long Variable::FreeParameters(
    const std::vector<Variable>& variables) const {
  std::vector<unsigned> high_bits = parent_set_.high_bits();
  auto parent = high_bits.cbegin();
  unsigned long long t = categories_.size() - 1;
  while (parent != high_bits.cend()) {
    t *= variables[*parent].categories_.size();
    parent++;
  }
  return t;
}

void Variable::InitializeVariables(const InvertedIndex& index,
                                   std::vector<Variable>& variables,
                                   std::vector<ExternalQueue>& external_queues,
                                   std::vector<Cache>& caches) {
  auto vart = index.index().cbegin();
  std::vector<std::set<unsigned> > categories(index.index().size());
  while (vart != index.index().cend()) {
    auto valuet = vart->second.entries().cbegin();
    while (valuet != vart->second.entries().cend()) {
      categories[vart->first].insert(valuet->first);
      ++valuet;
    }
    ++vart;
  }
  unsigned variable_id = 0;
  while (variable_id < categories.size()) {
    ExternalQueue* external_queue =
        (external_queues.empty()) ? NULL : &(external_queues[variable_id]);
    Cache* cache = (caches.empty()) ? NULL : &(caches[variable_id]);
    variables.emplace_back(variable_id, cache, external_queue,
                           index.index().size());
    auto category = categories[variable_id].cbegin();
    while (category != categories[variable_id].cend()) {
      variables[variable_id].mutable_categories().push_back(*category);
      ++category;
    }
    variables[variable_id].FindBestEntry();
    variable_id++;
  }
}

std::string Variable::ToString() const {
  std::stringstream str;
  str << "Id: " << variable_id_ << std::endl;
  str << "Categories:";
  auto category = categories_.cbegin();
  while (category != categories_.cend()) {
    str << " " << *category;
    ++category;
  }
  str << std::endl << "Parents:";
  std::vector<unsigned> high_bits = parent_set_.high_bits();
  if (high_bits.empty()) {
    str << " Empty";
  }
  auto parent = high_bits.cbegin();
  while (parent != high_bits.cend()) {
    str << " " << *parent;
    ++parent;
  }
  return str.str();
}

void Variable::FindBestEntry() {
  if (cache_ == NULL || cache_->cache().empty()) {
    return;
  }
  cache_->BestCompliantEntry(Bitset(parent_set_.bit_count()),
                             Bitset(parent_set_.bit_count()), parent_set_);
}

long double Variable::LLOuterSum(
    const InvertedIndex& index, const std::vector<Variable>& variables,
    const std::unordered_set<unsigned>& intersection,
    std::vector<unsigned>& configuration) const {
  if (intersection.empty() && !configuration.empty()) {
    return 0;
  }
  std::vector<unsigned> high_bits = parent_set_.high_bits();
  if (configuration.size() == high_bits.size()) {
    return LLInnerSum(index, intersection);
  }
  unsigned parent_id = high_bits[configuration.size()];
  const Variable& variable = variables[parent_id];
  auto category = variable.categories_.cbegin();
  long double summation = 0;
  while (category != variable.categories_.cend()) {
    std::unordered_set<unsigned> next_intersection;
    if (configuration.empty()) {
      next_intersection = index.at(variable.variable_id()).at(*category);
    } else {
      InvertedIndex::MinimalIntersection(
          intersection, index.at(variable.variable_id()).at(*category),
          next_intersection);
    }
    configuration.push_back(*category);
    summation += LLOuterSum(index, variables, next_intersection, configuration);
    configuration.pop_back();
    ++category;
  }
  return summation;
}

long double Variable::LLInnerSum(
    const InvertedIndex& index,
    const std::unordered_set<unsigned>& intersection) const {
  std::unordered_map<unsigned, unsigned> n_ijks(categories_.size());
  unsigned n_ij = 0;
  auto kt = index.at(variable_id_).entries().cbegin();
  while (kt != index.at(variable_id_).entries().cend()) {
    unsigned value = 0;
    if (intersection.empty()) {
      value = kt->second.size();
    } else {
      value = InvertedIndex::CountingIntersection(intersection, kt->second);
    }
    n_ij += value;
    n_ijks[kt->first] = value;
    ++kt;
  }
  long double score = 0;
  auto k = n_ijks.cbegin();
  while (k != n_ijks.cend()) {
    unsigned n_ijk = k->second;
    long double theta_ijk = n_ijk;
    if (n_ij && n_ijk) {
      theta_ijk /= (long double)n_ij;
      theta_ijk = log2(theta_ijk);
      theta_ijk *= (long double)n_ijk;
    } else {
      theta_ijk = 0;
    }
    score += theta_ijk;
    k++;
  }
  return score;
}

}  // namespce branch_and_bound
}  // namespace bayes
