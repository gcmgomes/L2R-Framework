// A possible optimization to compute parent set configurations.
#ifndef _L2RF_BAYES_BRANCH_AND_BOUND_INVERTED_INDEX_H_
#define _L2RF_BAYES_BRANCH_AND_BOUND_INVERTED_INDEX_H_

#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "instance.h"

namespace bayes {
namespace branch_and_bound {

class InvertedIndexEntry {
 public:
  InvertedIndexEntry();

  const std::unordered_map<unsigned, std::unordered_set<unsigned>>& listings()
      const {
    return listings_;
  }

  std::unordered_map<unsigned, std::unordered_set<unsigned>>&
  mutable_listings() {
    return listings_;
  }

  const std::unordered_set<unsigned>& at(unsigned category) const {
    return listings_.at(category);
  }

 private:
  std::unordered_map<unsigned, std::unordered_set<unsigned>> listings_;
};

class InvertedIndex {
 public:
  InvertedIndex(const std::vector<Instance>& instances);

  const std::map<unsigned, InvertedIndexEntry>& index() const {
    return index_;
  }

  unsigned Count(unsigned variable_id, unsigned variable_value) const;

  void ConfigurationIntersect(const std::set<unsigned>& variables,
                              const std::vector<unsigned>& configuration,
                              std::unordered_set<unsigned>& intersect) const;

  size_t BaseVariableIntersect(unsigned base_variable, unsigned base_value,
                               const std::unordered_set<unsigned>& intersect) const;

  unsigned TotalPoints() const;

  std::string ToString() const;

 private:
  // Initializes the inverted index.
  void Initialize(const std::vector<Instance>& instances);

  std::map<unsigned, InvertedIndexEntry> index_;
};

}  // namespce branch_and_bound
}  // namespace bayes

#endif
