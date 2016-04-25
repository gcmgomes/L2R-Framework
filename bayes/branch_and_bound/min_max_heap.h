// This hepa structure will act as a priority queue for the branch and bound
// proceedure. Experiments showed that the standard STL priority_queue is
// actually a very big memory hog, requiring well over three times the required
// space for a set of objects.
#ifndef _L2RF_BAYES_BRANCH_AND_BOUND_MIN_MAX_HEAP_H_
#define _L2RF_BAYES_BRANCH_AND_BOUND_MIN_MAX_HEAP_H_

#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>
#include "graph.h"

namespace bayes {
namespace branch_and_bound {

class MinMaxHeapNode {
 public:
  MinMaxHeapNode(const Graph& graph, MinMaxHeapNode* parent);

  const Graph& graph() const {
    return graph_;
  }

  Graph& mutable_graph() {
    return graph_;
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
    return graph_.score();
  }

  void Print(std::string left_padding) const;

 private:
  Graph graph_;

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

  void push(const Graph& graph);

  const Graph& max() const;

  const Graph& min() const;

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
