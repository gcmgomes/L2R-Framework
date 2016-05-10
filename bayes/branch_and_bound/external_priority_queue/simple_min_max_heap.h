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

namespace bayes {
namespace branch_and_bound {

class SimpleMinMaxHeapNode {
 public:
  SimpleMinMaxHeapNode(int key, SimpleMinMaxHeapNode* parent);

  int key() const {
    return key_;
  }

  int& mutable_key() {
    return key_;
  }

  SimpleMinMaxHeapNode* parent() const {
    return parent_;
  }

  SimpleMinMaxHeapNode* left() const {
    return left_.get();
  }

  std::unique_ptr<SimpleMinMaxHeapNode>& mutable_left() {
    return left_;
  }

  SimpleMinMaxHeapNode* right() const {
    return right_.get();
  }

  std::unique_ptr<SimpleMinMaxHeapNode>& mutable_right() {
    return right_;
  }

  int score() const {
    return key_;
  }

  void Print(std::string left_padding) const;

 private:
  int key_;

  SimpleMinMaxHeapNode* parent_;

  std::unique_ptr<SimpleMinMaxHeapNode> left_;

  std::unique_ptr<SimpleMinMaxHeapNode> right_;
};

class SimpleMinMaxHeap {
 public:
  SimpleMinMaxHeap();

  size_t size() const {
    return size_;
  }

  void push(int key);

  int max() const;

  int min() const;

  void pop_max();

  void pop_min();

  bool empty() const;

  void Print() const;

 private:
  void SimpleMinMaxHeapify(SimpleMinMaxHeapNode* node);

  // Node is always a max level node.
  void MaxBubbleUp(SimpleMinMaxHeapNode* node);

  // Node is always a min level node.
  void MinBubbleUp(SimpleMinMaxHeapNode* node);

  // Node is always a max level node.
  void MaxSiftDown(SimpleMinMaxHeapNode* node);

  // Node is always a min level node.
  void MinSiftDown(SimpleMinMaxHeapNode* node);

  SimpleMinMaxHeapNode* GetNode(unsigned node_id) const;

  size_t size_;
  std::unique_ptr<SimpleMinMaxHeapNode> root_;
};

}  // namespce branch_and_bound
}  // namespace bayes

#endif
