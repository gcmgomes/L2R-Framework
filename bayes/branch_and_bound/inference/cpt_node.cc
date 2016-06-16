#include "cpt_node.h"

namespace bayes {
namespace branch_and_bound {
namespace inference {

CPTNode::CPTNode() : CPTNode(0, 0, 0) {
}

CPTNode::CPTNode(unsigned variable_id, unsigned value, long double probability)
    : variable_id_(variable_id), value_(value), probability_(probability) {
}

static unsigned long long MakeKey(unsigned var_id, unsigned val) {
  unsigned long long key = var_id, k = val;
  key <<= 32;
  key |= k;
  return key;
}

const std::unique_ptr<CPTNode>& CPTNode::at(unsigned var_id,
                                            unsigned val) const {
  return children_.at(MakeKey(var_id, val));
}

void CPTNode::push_child(unsigned var_id, unsigned val) {
  unsigned long long key = MakeKey(var_id, val);
  children_[key].reset(new CPTNode(var_id, val, 0));
}

void CPTNode::RootSplit(std::vector<unsigned>& variable_set,
                        const InvertedIndex& index) {
  PopulateChildren(variable_set[0], index);
  for (auto& child : children_) {
    child.second->Split(0, variable_set, std::unordered_set<unsigned>(), index);
  }
}

void CPTNode::PopulateChildren(
    unsigned child_variable_id,
    const bayes::branch_and_bound::InvertedIndex& index) {
  const auto& entries = index.at(child_variable_id).entries();
  for (const auto& entry : entries) {
    this->push_child(child_variable_id, entry.first);
  }
}

long double CPTNode::Split(
    unsigned variable_position, std::vector<unsigned>& variable_set,
    const std::unordered_set<unsigned>& intersection,
    const bayes::branch_and_bound::InvertedIndex& index) {
  std::unordered_set<unsigned> current_intersection;
  if (variable_position) {
    bayes::branch_and_bound::InvertedIndex::MinimalIntersection(
        intersection, index.at(variable_id_).at(value_), current_intersection);
  } else {
    current_intersection = index.at(variable_id_).at(value_);
  }
  if (current_intersection.empty()) {
    probability_ = 0;
  } else if (variable_id_ != variable_set.back()) {
    variable_position++;
    PopulateChildren(variable_set[variable_position], index);
    for (auto& child : children_) {
      child.second->Split(variable_position, variable_set, current_intersection,
                          index);
    }
  } else {
    probability_ =
        (long double)current_intersection.size() / intersection.size();
  }
  return probability_;
}

}  // namespce inference
}  // namespce branch_and_bound
}  // namespace bayes
