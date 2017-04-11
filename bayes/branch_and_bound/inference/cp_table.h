#ifndef _RI_2015_1_BAYES_BRANCH_AND_BOUND_INFERENCE_CP_TABLE_H_
#define _RI_2015_1_BAYES_BRANCH_AND_BOUND_INFERENCE_CP_TABLE_H_

#include "../bitset.h"
#include "cpt_node.h"
#include <cstdlib>
#include <unordered_set>

namespace bayes {
namespace branch_and_bound {
namespace inference {

class CPTable {
 public:
  CPTable(unsigned variable_id, const Bitset& parent_set);

  void Build(const InvertedIndex& index);

  void Rebuild(const InvertedIndex& index, const Bitset& parent_set);

  long double Query(const Instance& instance) const;

  std::string ToString() const;

 private:
  unsigned variable_id_;
  Bitset parent_set_;
  std::unique_ptr<CPTNode> root_;
};

}  // namespce inference
}  // namespce branch_and_bound
}  // namespace bayes

#endif
