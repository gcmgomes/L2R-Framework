// This heap structure will act as a double ended priority queue for the branch
// and bound proceedure. Experiments showed that the standard STL multiset is
// a bit slow and not really designed for this task. More
// over, std::priority_queue does not support both top/max and bottom/min
// operations.
// Nodes on even levels of the heap are MIN nodes, the others are MAX nodes.
#ifndef _L2RF_BAYES_BRANCH_AND_BOUND_MIN_MAX_HEAP_H_
#define _L2RF_BAYES_BRANCH_AND_BOUND_MIN_MAX_HEAP_H_

#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>
#include "search_tree.h"

namespace bayes {
namespace branch_and_bound {

class MinMaxHeapNode {
 public:
  MinMaxHeapNode(SearchTreeNode* search_tree_node, MinMaxHeapNode* parent);

  SearchTreeNode* search_tree_node() const {
    return search_tree_node_;
  }

  SearchTreeNode*& mutable_search_tree_node() {
    return search_tree_node_;
  }

  MinMaxHeapNode* parent() const {
    return parent_;
  }

  MinMaxHeapNode* left() const {
    return left_.get();
  }

  std::unique_ptr<MinMaxHeapNode>& mutable_left() {
    return left_;
  }

  MinMaxHeapNode* right() const {
    return right_.get();
  }

  std::unique_ptr<MinMaxHeapNode>& mutable_right() {
    return right_;
  }

  long double score() const {
    return search_tree_node_->score();
  }

  void Print(std::string left_padding) const;

 private:
  SearchTreeNode* search_tree_node_;

  MinMaxHeapNode* parent_;

  std::unique_ptr<MinMaxHeapNode> left_;

  std::unique_ptr<MinMaxHeapNode> right_;
};

class MinMaxHeap {
 public:
  MinMaxHeap();

  size_t size() const {
    return size_;
  }

  void push(SearchTreeNode* search_tree_node);

  SearchTreeNode* max() const;

  SearchTreeNode* min() const;

  void pop_max();

  void pop_min();

  bool empty() const;

  void Print() const;

 private:
  void MinMaxHeapify(MinMaxHeapNode* node);

  // Node is always a max level node.
  void MaxBubbleUp(MinMaxHeapNode* node);

  // Node is always a min level node.
  void MinBubbleUp(MinMaxHeapNode* node);

  // Node is always a max level node.
  void MaxSiftDown(MinMaxHeapNode* node);

  // Node is always a min level node.
  void MinSiftDown(MinMaxHeapNode* node);

  MinMaxHeapNode* GetNode(unsigned node_id) const;

  size_t size_;
  std::unique_ptr<MinMaxHeapNode> root_;
};

}  // namespce branch_and_bound
}  // namespace bayes

#endif
