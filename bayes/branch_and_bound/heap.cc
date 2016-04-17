#include <algorithm>
#include <limits>
#include <sstream>
#include "heap.h"

namespace bayes {
namespace branch_and_bound {

HeapNode::HeapNode(const Graph& graph, HeapNode* parent)
    : graph_(graph), parent_(parent) {
}

Heap::Heap() : size_(0) {
}

static void Swap(Graph& a, Graph& b) {
  Graph tmp = a;
  a = b;
  b = tmp;
}

void Heap::push(const Graph& graph) {
  size_++;
  if (root_ == NULL) {
    root_.reset(new HeapNode(graph, NULL));
    return;
  }
  auto* node = LastNode(true);
  if (node->left() == NULL) {
    node->mutable_left().reset(new HeapNode(Graph(), node));
    node = node->left();
  } else if (node->right() == NULL) {
    node->mutable_right().reset(new HeapNode(Graph(), node));
    node = node->right();
  }
  Increase(node, graph);
}

void Heap::pop() {
  if (empty()) {
    return;
  } else if (size_ == 1) {
    size_--;
    root_.reset();
    return;
  }
  auto* node = LastNode(true);
  if(size_ & 1) {
    Swap(root_->mutable_graph(), node->right()->mutable_graph());
    node->mutable_right().reset();
  } else {
    Swap(root_->mutable_graph(), node->left()->mutable_graph());
    node->mutable_left().reset();
  }
  size_--;
  Heapify(root_.get());
}

bool Heap::empty() const {
  return size_ == 0;
}

void HeapNode::Print(std::string left_padding) const {
  std::cout << left_padding << score() << std::endl;
  left_padding += "\t";
  if (left_ == NULL) {
    std::cout << left_padding << "||" << std::endl;
  } else {
    left_->Print(left_padding);
  }
  if (right_ == NULL) {
    std::cout << left_padding << "||" << std::endl;
  } else {
    right_->Print(left_padding);
  }
}

void Heap::Print() const {
  if (root_ == NULL) {
    std::cout << "||" << std::endl;
    return;
  }
  root_->Print("");
}

void Heap::Heapify(HeapNode* node) {
  auto* largest = node;
  if (node->left() != NULL && node->left()->score() > largest->score()) {
    largest = node->left();
  }
  if (node->right() != NULL && node->right()->score() > largest->score()) {
    largest = node->right();
  }
  if (largest != node) {
    Swap(largest->mutable_graph(), node->mutable_graph());
    Heapify(largest);
  }
}

void Heap::Increase(HeapNode* node, const Graph& graph) {
  Graph current_graph = graph;
  if (graph.score() < node->score()) {
    return;
  }
  Swap(node->mutable_graph(), current_graph);
  while (node != NULL && node->parent() != NULL &&
         node->parent()->score() < node->score()) {
    Swap(node->mutable_graph(), node->parent()->mutable_graph());
    node = node->parent();
  }
}

HeapNode* Heap::LastNode(bool stop_on_penultimate) const {
  if (size_ <= 1) {
    return root_.get();
  }
  unsigned bits = log2(size_);
  auto* node = root_.get();
  size_t mask = 1 << (bits - 1);
  while (bits > stop_on_penultimate) {
    node = (size_ & mask) ? node->right() : node->left();
    mask >>= 1;
    bits--;
  }
  return node;
}

}  // namespce branch_and_bound
}  // namespace bayes
