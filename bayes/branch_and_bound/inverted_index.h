#ifndef _L2RF_BAYES_BRANCH_AND_BOUND_INVERTED_INDEX_H_
#define _L2RF_BAYES_BRANCH_AND_BOUND_INVERTED_INDEX_H_

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "instance.h"

namespace bayes {
namespace branch_and_bound {

class IndexEntry {
 public:
  IndexEntry() {
  }

  const std::unordered_map<unsigned, std::unordered_set<unsigned> >& entries()
      const {
    return entries_;
  }

  std::unordered_map<unsigned, std::unordered_set<unsigned> >&
  mutable_entries() {
    return entries_;
  }

  const std::unordered_set<unsigned>& at(unsigned value) const {
    return entries_.at(value);
  }

 private:
  std::unordered_map<unsigned, std::unordered_set<unsigned> > entries_;
};

class InvertedIndex {
 public:
  InvertedIndex(const std::vector<Instance>& instances);

  const std::unordered_map<unsigned, IndexEntry>& index() const {
    return index_;
  }

  const IndexEntry& at(unsigned variable) const {
    return index_.at(variable);
  }

  static void MinimalIntersection(const std::unordered_set<unsigned>& first,
                                  const std::unordered_set<unsigned>& second,
                                  std::unordered_set<unsigned>& intersection);

  static unsigned CountingIntersection(
      const std::unordered_set<unsigned>& first,
      const std::unordered_set<unsigned>& second);

  std::string ToString() const;

  void clear() {
    index_.clear();
  }

 private:
  void Initialize(const std::vector<Instance>& instances);

  std::unordered_map<unsigned, IndexEntry> index_;
};

}  // namespce branch_and_bound
}  // namespace bayes

#endif
