#include <sstream>
#include "inverted_index.h"

namespace bayes {
namespace branch_and_bound {

InvertedIndex::InvertedIndex(const std::vector<Instance>& instances) {
  Initialize(instances);
}

void InvertedIndex::MinimalIntersection(
    const std::unordered_set<unsigned>& first,
    const std::unordered_set<unsigned>& second,
    std::unordered_set<unsigned>& intersection) {
  const auto& smaller = (first.size() < second.size()) ? first : second;
  const auto& greater = (first.size() < second.size()) ? second : first;
  intersection.clear();
  auto it = smaller.cbegin();
  while (it != smaller.cend()) {
    if (greater.count(*it)) {
      intersection.insert(*it);
    }
    ++it;
  }
}

unsigned InvertedIndex::CountingIntersection(
    const std::unordered_set<unsigned>& first,
    const std::unordered_set<unsigned>& second) {
  const auto& smaller = (first.size() < second.size()) ? first : second;
  const auto& greater = (first.size() < second.size()) ? second : first;
  auto it = smaller.cbegin();
  unsigned count = 0;
  while (it != smaller.cend()) {
    if (greater.count(*it)) {
      ++count;
    }
    ++it;
  }
  return count;
}

std::string InvertedIndex::ToString() const {
  auto it = index_.cbegin();
  std::stringstream str;
  while (it != index_.cend()) {
    auto jt = it->second.entries().cbegin();
    while (jt != it->second.entries().cend()) {
      auto kt = jt->second.cbegin();
      str << it->first << ", " << jt->first << ":";
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
      index_[variable]
          .mutable_entries()[instance->values().at(variable)]
          .insert(instance_id);
      variable++;
    }
    instance_id++;
    ++instance;
  }
}

}  // namespace branch_and_bound
}  // namespace bayes
