// This hepa structure will act as a priority queue for the branch and bound
// proceedure. Experiments showed that the standard STL priority_queue is
// actually a very big memory hog, requiring well over three times the required
// space for a set of objects.
#ifndef _L2RF_BAYES_BRANCH_AND_BOUND_HEAP_H_
#define _L2RF_BAYES_BRANCH_AND_BOUND_HEAP_H_

#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>
#include "graph.h"

namespace bayes {
namespace branch_and_bound {

class HeapNode {
 public:
  HeapNode(const Graph& graph, HeapNode* parent);

  const Graph& graph() const {
    return graph_;
  }

  Graph& mutable_graph() {
    return graph_;
  }

  HeapNode* parent() {
    return parent_;
  }

  HeapNode* left() {
    return left_.get();
  }

  std::unique_ptr<HeapNode>& mutable_left() {
    return left_;
  }

  HeapNode* right() {
    return right_.get();
  }

  std::unique_ptr<HeapNode>& mutable_right() {
    return right_;
  }

  long double score() const {
    return graph_.score();
  }

  void Print(std::string left_padding) const;

 private:
  Graph graph_;

  HeapNode* parent_;

  std::unique_ptr<HeapNode> left_;

  std::unique_ptr<HeapNode> right_;
};

class Heap {
 public:
  Heap();

  size_t size() const {
    return size_;
  }

  void push(const Graph& graph);

  void pop();

  bool empty() const;

  const Graph& top() const {
    return root_->graph();
  }

  void Print() const;

 private:
  void Heapify(HeapNode* node);

  void Increase(HeapNode* node, const Graph& graph);

  HeapNode* LastNode(bool stop_on_penultimate = false) const;

  size_t size_;
  std::unique_ptr<HeapNode> root_;
};

}  // namespce branch_and_bound
}  // namespace bayes

#endif
