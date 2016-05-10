// This heap structure will act as a double ended priority queue for the branch
// and bound proceedure. Experiments showed that the standard STL multiset is
// a bit slow and not really designed for this task. More
// over, std::priority_queue does not support both top/max and bottom/min
// operations.
// Nodes on even levels of the heap are MIN nodes, the others are MAX nodes.
#ifndef _L2RF_BAYES_BRANCH_AND_BOUND_SIMPLE_MIN_MAX_HEAP_H_
#define _L2RF_BAYES_BRANCH_AND_BOUND_SIMPLE_MIN_MAX_HEAP_H_

#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>
#include "../graph.h"

namespace bayes {
namespace branch_and_bound {

class GraphMinMaxHeapNode {
 public:
  GraphMinMaxHeapNode(const Graph& key, GraphMinMaxHeapNode* parent);

  const std::unique_ptr<Graph>& key() const {
    return key_;
  }

  std::unique_ptr<Graph>& mutable_key() {
    return key_;
  }

  GraphMinMaxHeapNode* parent() const {
    return parent_;
  }

  GraphMinMaxHeapNode* left() const {
    return left_.get();
  }

  std::unique_ptr<GraphMinMaxHeapNode>& mutable_left() {
    return left_;
  }

  GraphMinMaxHeapNode* right() const {
    return right_.get();
  }

  std::unique_ptr<GraphMinMaxHeapNode>& mutable_right() {
    return right_;
  }

  long double score() const {
    return key_->score();
  }

  void Print(std::string left_padding) const;

 private:
  std::unique_ptr<Graph> key_;

  GraphMinMaxHeapNode* parent_;

  std::unique_ptr<GraphMinMaxHeapNode> left_;

  std::unique_ptr<GraphMinMaxHeapNode> right_;
};

class GraphMinMaxHeap {
 public:
  GraphMinMaxHeap();

  size_t size() const {
    return size_;
  }

  void push(const Graph& key);

  const Graph& max() const;

  const Graph& min() const;

  void pop_max();

  void pop_min();

  bool empty() const;

  void Print() const;

 private:
  void GraphMinMaxHeapify(GraphMinMaxHeapNode* node);

  // Node is always a max level node.
  void MaxBubbleUp(GraphMinMaxHeapNode* node);

  // Node is always a min level node.
  void MinBubbleUp(GraphMinMaxHeapNode* node);

  // Node is always a max level node.
  void MaxSiftDown(GraphMinMaxHeapNode* node);

  // Node is always a min level node.
  void MinSiftDown(GraphMinMaxHeapNode* node);

  GraphMinMaxHeapNode* GetNode(unsigned node_id) const;

  size_t size_;
  std::unique_ptr<GraphMinMaxHeapNode> root_;
};

}  // namespce branch_and_bound
}  // namespace bayes

#endif
