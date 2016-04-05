#include <queue>
#include <sstream>
#include <unordered_map>
#include "external_queue.h"
#include "variable.h"

namespace bayes {
namespace branch_and_bound {

Variable::Variable(unsigned variable_id, Cache* cache)
    : variable_id_(variable_id), cache_(cache) {
}

static void AugmentSupersets(
    unsigned variable_id, long double w, const std::vector<Variable>& variables,
    std::unordered_map<Bitset, std::pair<unsigned, CacheEntry> >&
        best_subset_entries,
    Bitset& parent_set, ExternalQueue& known_supersets) {
  auto& subset_entry = best_subset_entries[parent_set].second;
  auto variable = variables.cbegin();
  while (variable != variables.cend()) {
    unsigned id = variable->variable_id();
    if (id != variable_id && !parent_set.high_bits().count(id)) {
      parent_set.Set(id, true);
      if (!best_subset_entries.count(parent_set)) {
        known_supersets.push(parent_set);
        best_subset_entries[parent_set].first = parent_set.high_bits().size();
        best_subset_entries[parent_set].second = subset_entry;
      } else if (best_subset_entries[parent_set].second.score(w) <
                 subset_entry.score(w)) {
        best_subset_entries[parent_set].second = subset_entry;
      }
      best_subset_entries[parent_set].first--;
      parent_set.Set(id, false);
    }
    ++variable;
  }
}

long double Variable::score() const {
  return cache_->at(parent_set_).score(cache_->w());
}

void Variable::BuildCache(const std::vector<Instance>& instances,
                          const std::vector<Variable>& variables) {
  unsigned long long evaluated = 0, discarded = 0;
  long double w = cache_->w();
  parent_set_ = Bitset(variables.size());
  ExternalQueue known_supersets(
      "/Users/gcmgomes/git_repositories/learning_to_rank_parse/bayes/"
      "branch_and_bound/tmp/test_queue.txt",
      100000);
  std::unordered_map<Bitset, std::pair<unsigned, CacheEntry> >
      best_subset_entries;
  known_supersets.push(parent_set_);
  while (!known_supersets.empty()) {
    parent_set_ = known_supersets.front();
    known_supersets.pop();
    std::cerr << "\rQueue size: " << known_supersets.size()
              << " Evaluated: " << ++evaluated << " Discarded: " << discarded
              << " Cache size: " << cache_->cache().size()
              << " Set_size: " << parent_set_.high_bits().size()
              << "                ";
    bool is_empty_set = parent_set_.high_bits().empty();
    unsigned long long free_parameters = FreeParameters(variables);
    auto& subset_entry = best_subset_entries[parent_set_].second;
    bool missing_edges = best_subset_entries[parent_set_].first;
    if ((is_empty_set || free_parameters + subset_entry.score(w) <= 0) &&
        !missing_edges) {  // Passed Lemma 2
      long double log_likelihood = LogLikelihood(instances, variables);
      CacheEntry entry(log_likelihood, free_parameters);
      if (is_empty_set) {
        best_subset_entries[parent_set_].second = entry;
      }
      if (is_empty_set ||
          subset_entry.score(w) <= entry.score(w)) {  // Passed Lemma 1
        cache_->Insert(parent_set_, entry);
        if (cache_->cache().size() >= 100000) {
          cache_->WriteToRepository();
        }
        AugmentSupersets(variable_id_, w, variables, best_subset_entries,
                         parent_set_, known_supersets);
      } else {
        discarded++;
      }
    } else {
      discarded++;
    }
    best_subset_entries.erase(parent_set_);
  }
  auto it = cache_->cache().cbegin();
  CacheEntry best_entry = it->second;
  while (it != cache_->cache().cend()) {
    if (best_entry.score(cache_->w()) <= it->second.score(cache_->w())) {
      best_entry = it->second;
      parent_set_ = it->first;
    }
    ++it;
  }

  if (false) {
    std::cerr << std::endl
              << "##########################################" << std::endl;
    std::cerr << "Final cache for variable " << variable_id_ << "has "
              << cache_->cache().size() << " entries: " << std::endl;
    it = cache_->cache().cbegin();
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
  cache_->WriteToRepository();
}

long double Variable::LogLikelihood(
    const std::vector<Instance>& instances,
    const std::vector<Variable>& variables) const {
  std::vector<unsigned> configuration;
  return LLOuterSum(instances, variables, configuration);
}

unsigned long long Variable::FreeParameters(
    const std::vector<Variable>& variables) const {
  auto parent = parent_set_.high_bits().cbegin();
  unsigned long long t = categories_.size() - 1;
  while (parent != parent_set_.high_bits().cend()) {
    t *= variables[*parent].categories_.size();
    parent++;
  }
  return t;
}

void Variable::InitializeVariables(const std::vector<Instance>& instances,
                                   std::vector<Variable>& variables,
                                   std::vector<Cache>& caches,
                                   Criterion criterion) {
  long double w = 1;
  if (criterion == Criterion::MINIMUM_DESCRIPTION_LEGNTH) {
    w = log2(instances.size()) / 2.0;
  }
  auto instance = instances.cbegin();
  std::vector<std::set<unsigned> > categories(instance->values().size());
  while (instance != instances.cend()) {
    unsigned variable = 0;
    while (variable < instance->values().size()) {
      categories[variable].insert(instance->values().at(variable));
      caches.emplace_back(w);
      variable++;
    }
    ++instance;
  }
  unsigned variable_id = 0;
  while (variable_id < categories.size()) {
    variables.emplace_back(variable_id, &caches[variable_id]);
    auto category = categories[variable_id].cbegin();
    while (category != categories[variable_id].cend()) {
      variables.back().mutable_categories().push_back(*category);
      ++category;
    }
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
  if (parent_set_.high_bits().empty()) {
    str << " Empty";
  }
  auto parent = parent_set_.high_bits().cbegin();
  while (parent != parent_set_.high_bits().cend()) {
    str << " " << *parent;
    ++parent;
  }
  return str.str();
}

long double Variable::LLOuterSum(const std::vector<Instance>& instances,
                                 const std::vector<Variable>& variables,
                                 std::vector<unsigned>& configuration) const {
  if (configuration.size() == parent_set_.high_bits().size()) {
    return LLInnerSum(instances, configuration);
  }
  unsigned parent_id = 0;
  auto parent = parent_set_.high_bits().cbegin();
  while (parent_id < configuration.size()) {
    parent_id++;
    ++parent;
  }
  const Variable& variable = variables[*parent];
  auto category = variable.categories_.cbegin();
  long double summation = 0;
  while (category != variable.categories_.cend()) {
    configuration.push_back(*category);
    summation += LLOuterSum(instances, variables, configuration);
    configuration.pop_back();
    ++category;
  }
  return summation;
}

long double Variable::LLInnerSum(const std::vector<Instance>& instances,
                                 std::vector<unsigned>& configuration) const {
  auto instance = instances.cbegin();
  std::unordered_map<unsigned, unsigned> n_ijks(categories_.size());
  unsigned n_ij = 0;
  while (instance != instances.cend()) {
    bool increment = true;
    auto parent = parent_set_.high_bits().cbegin();
    unsigned p_i = 0;
    while (parent != parent_set_.high_bits().cend()) {
      if (instance->values().at(*parent) != configuration[p_i]) {
        increment = false;
        break;
      }
      p_i++;
      ++parent;
    }
    n_ij += increment;
    n_ijks[instance->values().at(variable_id_)] += increment;
    ++instance;
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
