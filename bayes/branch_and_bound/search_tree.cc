#include <algorithm>
#include <limits>
#include <sstream>
#include "search_tree.h"

namespace bayes {
namespace branch_and_bound {

SearchTreeNode::SearchTreeNode(const Graph& graph, SearchTreeNode* parent) {
  score_ = graph.score();
  h_matrix_ = graph.h_matrix();
  parent_ = parent;
  modified_variable_ = -1;
}

SearchTreeNode::SearchTreeNode(const Graph& graph, unsigned modified_variable,
                               SearchTreeNode* parent) {
  score_ = graph.score();
  h_matrix_ = graph.h_matrix();
  parent_ = parent;
  modified_variable_ = modified_variable;
  modified_parent_set_ = graph.variables().at(modified_variable).parent_set();
}

SearchTreeNode* SearchTreeNode::AddChild(const Graph& graph,
                                         unsigned modified_variable) {
  children_.push_back(NULL);
  children_.back().reset(new SearchTreeNode(graph, modified_variable, this));
  return children_.back().get();
}

void SearchTreeNode::ClearMatrix() {
  h_matrix_.clear();
}

SearchTree::SearchTree(const Graph& base_graph) {
  base_graph_ = base_graph;
  root_.reset(new SearchTreeNode(base_graph, NULL));
}

void SearchTree::RebuildGraph(SearchTreeNode* leaf_node, Graph& graph) {
  graph = base_graph_;
  graph.mutable_h_matrix() = leaf_node->h_matrix();
  std::vector<bool> free_variables(graph.variables().size(), true);
  auto* node = leaf_node;
  while (node != root_.get()) {
    unsigned modified_variable = node->modified_variable();
    if(modified_variable == -1) {
      break;
    }
    if (free_variables[modified_variable]) {
      free_variables[modified_variable] = false;
      graph.mutable_variables()[modified_variable].mutable_parent_set() =
          node->modified_parent_set();
    }
    node = node->parent();
  }
  graph.RecomputeScore();
}

}  // namespce branch_and_bound
}  // namespace bayes
