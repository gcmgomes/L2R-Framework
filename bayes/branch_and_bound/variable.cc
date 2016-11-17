#include "variable.h"
#include <cassert>
#include <cmath>
#include <queue>
#include <sstream>
#include <unordered_map>

namespace bayes {
namespace branch_and_bound {

Variable::Variable(unsigned variable_id, const Bitset& parent_set, Cache* cache,
                   inference::CPTable* cp_table) {
  variable_id_ = variable_id;
  cache_ = cache;
  cp_table_ = cp_table;
  parent_set_ = parent_set;
}

long double Variable::score() const {
  if (cache_->cache().find(parent_set_) == cache_->cache().end()) {
  //if(cache_->cache().empty()) {
    return 0;
  }
  return cache_->at(parent_set_).score(cache_->w());
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
                                   std::vector<Cache>& caches,
                                   std::vector<inference::CPTable>& cp_tables) {
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
    Cache* cache = (caches.empty()) ? NULL : &(caches[variable_id]);
    inference::CPTable* cp_table =
        (cp_tables.empty()) ? NULL : &(cp_tables[variable_id]);
    variables.emplace_back(variable_id, Bitset(variables.size()), cache,
                           cp_table);
    auto category = categories[variable_id].cbegin();
    while (category != categories[variable_id].cend()) {
      variables[variable_id].mutable_categories().push_back(*category);
      ++category;
    }

    long double log_likelihood =
        variables[variable_id].LogLikelihood(index, variables);
    unsigned long long freeparameters =
        variables[variable_id].FreeParameters(variables);

    CacheEntry entry(log_likelihood, freeparameters);
    variables[variable_id].cache_->Insert(variables[variable_id].parent_set(),
                                          entry);

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
