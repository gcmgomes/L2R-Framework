#include <algorithm>
#include <limits>
#include <sstream>
#include "search_tree.h"

namespace bayes {
namespace branch_and_bound {

SearchTreeNode::SearchTreeNode(const Graph& graph, SearchTreeNode* parent)
    : SearchTreeNode(graph, -1, -1, parent) {
}

SearchTreeNode::SearchTreeNode(const Graph& graph, unsigned parent_variable,
                               unsigned child_variable,
                               SearchTreeNode* parent) {
  score_ = graph.score();
  parent_ = parent;
  modified_variable_ = child_variable;
  if(child_variable < graph.variables().size()) {
    modified_parent_set_ = graph.variables().at(child_variable).parent_set();
  }
  matrix_entry_ = MatrixEntry(parent_variable, child_variable);
}

SearchTreeNode* SearchTreeNode::AddChild(const Graph& graph,
                                         unsigned parent_variable,
                                         unsigned child_variable) {
  children_.push_back(NULL);
  children_.back().reset(
      new SearchTreeNode(graph, parent_variable, child_variable, this));
  return children_.back().get();
}

SearchTree::SearchTree(const Graph& base_graph) {
  base_graph_ = base_graph;
  root_.reset(new SearchTreeNode(base_graph, NULL));
}

void SearchTree::RebuildGraph(SearchTreeNode* leaf_node, Graph& graph) {
  graph = base_graph_;
  std::vector<bool> free_variables(graph.variables().size(), true);
  std::vector<SearchTreeNode*> leaf_to_root_path;
  auto* node = leaf_node;
  while (node != root_.get()) {
    unsigned modified_variable = node->modified_variable();
    if (free_variables[modified_variable]) {
      free_variables[modified_variable] = false;
      graph.mutable_variables()[modified_variable].mutable_parent_set() =
          node->modified_parent_set();
    }
    leaf_to_root_path.push_back(node);
    node = node->parent();
  }
  unsigned node_path_id = leaf_to_root_path.size();
  while (node_path_id > 1) {
    node_path_id--;
    node = leaf_to_root_path[node_path_id];
    auto* next = leaf_to_root_path[node_path_id - 1];
    auto it = node->children().cbegin();
    while (it != node->children().cend()) {
      unsigned child = (*it)->matrix_entry().child();
      unsigned parent = (*it)->matrix_entry().parent();
      if (it->get() != next) {
        graph.mutable_h_matrix()[parent][child] = ArcStatus::PRESENT;
        graph.mutable_h_matrix()[child][parent] = ArcStatus::PROHIBITED;
      } else {
        graph.mutable_h_matrix()[parent][child] = ArcStatus::PROHIBITED;
        break;
      }
      ++it;
    }
  }
  graph.RecomputeScore();
}

}  // namespce branch_and_bound
}  // namespace bayes
