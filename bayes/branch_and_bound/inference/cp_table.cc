#include "cp_table.h"
#include <sstream>
#include <stack>

namespace bayes {
namespace branch_and_bound {
namespace inference {

CPTable::CPTable(unsigned variable_id, const Bitset& parent_set)
    : variable_id_(variable_id), parent_set_(parent_set),
      root_(new CPTNode(-1, -1, -1)) {
}

void CPTable::Build(const InvertedIndex& index) {
  std::vector<unsigned> variable_set = parent_set_.high_bits();
  variable_set.push_back(variable_id_);
  root_->RootSplit(variable_set, index);
}

void CPTable::Rebuild(const InvertedIndex& index, const Bitset& parent_set) {
  parent_set_ = parent_set;
  root_.reset(new CPTNode(-1, -1, -1));
  Build(index);
}

long double CPTable::Query(const Instance& instance) const {
  std::vector<unsigned> variable_set = parent_set_.high_bits();
  variable_set.push_back(variable_id_);
  CPTNode* node = root_.get();
  for (const auto& var_id : variable_set) {
    if (node->count(var_id, instance.values().at(var_id))) {
      node = node->at(var_id, instance.values().at(var_id)).get();
    } else {
      node = NULL;
    }
    if (node == NULL) {
      break;
    }
  }
  return (node == NULL) ? 0 : node->probability();
}

std::string CPTable::ToString() const {
  std::stringstream stream;
  std::stack<std::pair<CPTNode*, std::string>> stack;
  stack.push(std::make_pair(root_.get(), std::string()));
  while (!stack.empty()) {
    std::string str = stack.top().second;
    CPTNode* node = stack.top().first;
    stack.pop();
    if (node->variable_id() != -1) {
      std::stringstream var_info;
      var_info << node->variable_id() << ":" << node->value() << " ";
      str += var_info.str();
    }
    if (node->children().empty()) {
      stream << str << node->probability() << std::endl;
    }
    for (const auto& child : node->children()) {
      stack.push(std::make_pair(child.second.get(), str));
    }
  }
  return stream.str();
}

}  // namespce inference
}  // namespce branch_and_bound
}  // namespace bayes
