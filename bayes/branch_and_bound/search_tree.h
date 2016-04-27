// This hepa structure will act as a priority queue for the branch and bound
// proceedure. Experiments showed that the standard STL priority_queue is
// actually a very big memory hog, requiring well over three times the required
// space for a set of objects.
#ifndef _L2RF_BAYES_BRANCH_AND_BOUND_SEARCH_TREE_H_
#define _L2RF_BAYES_BRANCH_AND_BOUND_SEARCH_TREE_H_

#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>
#include "graph.h"

namespace bayes {
namespace branch_and_bound {

class SearchTreeNode {
 public:
  SearchTreeNode(const Graph& graph, SearchTreeNode* parent);

  SearchTreeNode(const Graph& graph, unsigned modified_variable,
                 SearchTreeNode* parent);

  long double score() const {
    return score_;
  }

  SearchTreeNode* parent() const {
    return parent_;
  }

  unsigned modified_variable() const {
    return modified_variable_;
  }

  const Bitset& modified_parent_set() const {
    return modified_parent_set_;
  }

  const std::unordered_map<unsigned, std::unordered_map<unsigned, ArcStatus>>&
  h_matrix() const {
    return h_matrix_;
  }

  // Returns a pointer to the added child.
  SearchTreeNode* AddChild(const Graph& graph, unsigned modified_variable);

  // Erases |h_matrix_| to save up space.
  void ClearMatrix();

 private:
  long double score_;

  SearchTreeNode* parent_;

  std::vector<std::unique_ptr<SearchTreeNode>> children_;

  std::unordered_map<unsigned, std::unordered_map<unsigned, ArcStatus>>
      h_matrix_;

  unsigned modified_variable_;
  Bitset modified_parent_set_;
};

class SearchTree {
 public:
  // |base_graph| MUST be fully built before calling this constructor. That is,
  // it must have the optimal, unconstrained, parent sets for each variable
  // already calculated!
  SearchTree(const Graph& base_graph);

  SearchTreeNode* root() const {
    return root_.get();
  }

  // Perform a bottom-up traversal of the search tree to reconstruct the
  // graph determined by |leaf_mode.h_matrix()|. Stores the result in |graph|.
  void RebuildGraph(SearchTreeNode* leaf_node, Graph& graph);

 private:
  Graph base_graph_;
  std::unique_ptr<SearchTreeNode> root_;
};

}  // namespce branch_and_bound
}  // namespace bayes

#endif
