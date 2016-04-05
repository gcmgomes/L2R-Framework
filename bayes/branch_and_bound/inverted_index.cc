#include <sstream>
#include "inverted_index.h"

namespace bayes {

namespace branch_and_bound {

InvertedIndexEntry::InvertedIndexEntry() {
}

InvertedIndex::InvertedIndex(const std::vector<Instance>& instances) {
  Initialize(instances);
}

static void QuickIntersect(std::unordered_set<unsigned>& intersect,
                           const std::unordered_set<unsigned>& other) {
  auto it = intersect.begin();
  while (it != intersect.end()) {
    unsigned val = *it;
    ++it;
    if (!other.count(val)) {
      intersect.erase(val);
    }
  }
}

unsigned InvertedIndex::Count(unsigned variable_id, unsigned variable_value) const {
  return index_.at(variable_id).at(variable_value).size();
}

void InvertedIndex::ConfigurationIntersect(
    const std::set<unsigned>& variables,
    const std::vector<unsigned>& configuration,
    std::unordered_set<unsigned>& intersect) const {
  intersect.clear();
  if (variables.empty()) {
    return;
  }
  auto it = variables.cbegin();
  unsigned position = 0;
  intersect = index_.at(*it).at(configuration.at(position));
  ++it;
  position++;
  while (it != variables.cend()) {
    QuickIntersect(intersect, index_.at(*it).at(configuration.at(position)));
    ++it;
    position++;
  }
}

size_t InvertedIndex::BaseVariableIntersect(
    unsigned base_variable, unsigned base_value,
    const std::unordered_set<unsigned>& intersect) const {
  auto it = intersect.cbegin();
  size_t result = 0;
  while(it != intersect.cend()) {
    result += index_.at(base_variable).at(base_value).count(*it);
    ++it;
  }
  return result;
}

unsigned InvertedIndex::TotalPoints() const {
  auto it = index_.cbegin()->second.listings().cbegin();
  unsigned total = 0;
  while(it != index_.cbegin()->second.listings().cend()) {
    total += it->second.size();
    ++it;
  }
  return total;
}

std::string InvertedIndex::ToString() const {
  std::stringstream str;
  auto it = index_.cbegin();
  while (it != index_.cend()) {
    auto jt = it->second.listings().cbegin();
    while (jt != it->second.listings().cend()) {
      str << it->first << ", " << jt->first << ":";
      auto kt = jt->second.cbegin();
      while (kt != jt->second.cend()) {
        str << " " << *kt;
        ++kt;
      }
      str << std::endl;
      ++jt;
    }
    ++it;
  }
  return str.str();
}

void InvertedIndex::Initialize(const std::vector<Instance>& instances) {
  unsigned instance_id = 0;
  auto instance = instances.cbegin();
  while (instance != instances.cend()) {
    unsigned variable = 0;
    while (variable < instance->values().size()) {
      int value = instance->values().at(variable);
      index_[variable].mutable_listings()[value].insert(instance_id);
      variable++;
    }
    instance_id++;
    ++instance;
  }
}

}  // namespce branch_and_bound
}  // namespace bayes
