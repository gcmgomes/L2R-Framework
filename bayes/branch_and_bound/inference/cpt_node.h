#ifndef _RI_2015_1_BAYES_BRANCH_AND_BOUND_INFERENCE_CPT_NODE_H_
#define _RI_2015_1_BAYES_BRANCH_AND_BOUND_INFERENCE_CPT_NODE_H_

#include "../inverted_index.h"
#include <cstdlib>
#include <iostream>
#include <unordered_map>

namespace bayes {
namespace branch_and_bound {
namespace inference {

class CPTNode {
 public:
  CPTNode();

  CPTNode(unsigned variable_id, unsigned value, long double probability);

  unsigned variable_id() const {
    return variable_id_;
  }

  unsigned value() const {
    return value_;
  }

  long double probability() const {
    return probability_;
  }

  const std::unordered_map<unsigned long long, std::unique_ptr<CPTNode>>&
  children() const {
    return children_;
  }

  std::unordered_map<unsigned long long, std::unique_ptr<CPTNode>>&
  mutable_children() {
    return children_;
  }

  const std::unique_ptr<CPTNode>& at(unsigned var_id, unsigned val) const;

  void push_child(unsigned var_id, unsigned val);

  void RootSplit(std::vector<unsigned>& variable_set,
                 const bayes::branch_and_bound::InvertedIndex& index);

 private:
  void PopulateChildren(unsigned child_variable_id,
                        const bayes::branch_and_bound::InvertedIndex& index);

  // The last element of |variable_set| must be the variable that holds the CPT.
  long double Split(unsigned variable_position,
                    std::vector<unsigned>& variable_set,
                    const std::unordered_set<unsigned>& intersection,
                    const bayes::branch_and_bound::InvertedIndex& index);

  unsigned variable_id_;

  unsigned value_;

  double probability_;

  std::unordered_map<unsigned long long, std::unique_ptr<CPTNode>> children_;
};

}  // namespce inference
}  // namespce branch_and_bound
}  // namespace bayes

#endif
