#include <algorithm>
#include <limits>
#include <sstream>
#include "min_max_heap.h"

namespace bayes {
namespace branch_and_bound {

MinMaxHeapNode::MinMaxHeapNode(SearchTreeNode* search_tree_node,
                               MinMaxHeapNode* parent)
    : search_tree_node_(search_tree_node), parent_(parent) {
}

MinMaxHeap::MinMaxHeap() : size_(0) {
}

static void Swap(MinMaxHeapNode* a, MinMaxHeapNode* b) {
  SearchTreeNode* tmp = a->search_tree_node();
  a->mutable_search_tree_node() = b->search_tree_node();
  b->mutable_search_tree_node() = tmp;
}

void MinMaxHeap::push(SearchTreeNode* search_tree_node) {
  size_++;
  if (size_ == 1) {
    root_.reset(new MinMaxHeapNode(search_tree_node, NULL));
    return;
  }
  auto* node = GetNode(size_ >> 1);
  if (node->left() == NULL) {
    node->mutable_left().reset(new MinMaxHeapNode(search_tree_node, node));
    node = node->left();
  } else {
    node->mutable_right().reset(new MinMaxHeapNode(search_tree_node, node));
    node = node->right();
  }
  MinMaxHeapify(node);
}

SearchTreeNode* MinMaxHeap::max() const {
  if (size_ <= 1) {
    return root_->search_tree_node();
  } else if (size_ == 2) {
    return root_->left()->search_tree_node();
  } else if (root_->left()->score() > root_->right()->score()) {
    return root_->left()->search_tree_node();
  }
  return root_->right()->search_tree_node();
}

SearchTreeNode* MinMaxHeap::min() const {
  return root_->search_tree_node();
}

void MinMaxHeap::pop_max() {
  if (size_ <= 1) {
    size_ = 0;
    root_.reset();
    return;
  }
  if (size_ == 2) {
    size_--;
    root_->mutable_left().reset();
    return;
  }
  auto* maximum = (root_->left()->score() > root_->right()->score())
                      ? root_->left()
                      : root_->right();
  auto* penultimate_node = GetNode(size_ >> 1);
  if (size_ & 1) {
    Swap(maximum, penultimate_node->right());
    penultimate_node->mutable_right().reset();
  } else {
    Swap(maximum, penultimate_node->left());
    penultimate_node->mutable_left().reset();
  }
  size_--;
  MaxSiftDown(maximum);
}

void MinMaxHeap::pop_min() {
  if (size_ <= 1) {
    size_ = 0;
    root_.reset();
    return;
  }
  auto* penultimate_node = GetNode(size_ >> 1);
  if (size_ & 1) {
    // std::cout << "Swapping " << root_->score() << " with "
    //           << penultimate_node->right()->score() << std::endl;
    Swap(root_.get(), penultimate_node->right());
    penultimate_node->mutable_right().reset();
  } else {
    // std::cout << "Swapping " << root_->score() << " with "
    //           << penultimate_node->left()->score() << std::endl;
    Swap(root_.get(), penultimate_node->left());
    penultimate_node->mutable_left().reset();
  }
  size_--;
  MinSiftDown(root_.get());
}

bool MinMaxHeap::empty() const {
  return size_ == 0;
}

void MinMaxHeapNode::Print(std::string left_padding) const {
  std::cout << left_padding << score() << std::endl;
  left_padding += "\t";
  if (right_ == NULL) {
    std::cout << left_padding << "||" << std::endl;
  } else {
    right_->Print(left_padding);
  }
  if (left_ == NULL) {
    std::cout << left_padding << "||" << std::endl;
  } else {
    left_->Print(left_padding);
  }
}

void MinMaxHeap::Print() const {
  if (root_ == NULL) {
    std::cout << "||" << std::endl;
    return;
  }
  root_->Print("");
}

void MinMaxHeap::MinMaxHeapify(MinMaxHeapNode* node) {
  bool is_max_node = ((unsigned)log2(size_)) % 2;
  if (is_max_node) {
    MaxBubbleUp(node);
  } else {
    MinBubbleUp(node);
  }
}

static MinMaxHeapNode* Grandparent(MinMaxHeapNode* node) {
  if (node != NULL && node->parent() != NULL &&
      node->parent()->parent() != NULL) {
    return node->parent()->parent();
  }
  return NULL;
}

void MinMaxHeap::MaxBubbleUp(MinMaxHeapNode* node) {
  if (node->parent() == NULL) {
    return;
  }
  // Make a child-parent swap and MinBubbleUp.
  if (node->parent()->score() > node->score()) {
    Swap(node->parent(), node);
    MinBubbleUp(node->parent());
    return;
  }
  auto* grandparent = Grandparent(node);
  while (grandparent != NULL && grandparent->score() < node->score()) {
    Swap(grandparent, node);
    node = grandparent;
    grandparent = Grandparent(grandparent);
  }
}

void MinMaxHeap::MinBubbleUp(MinMaxHeapNode* node) {
  if (node->parent() == NULL) {
    return;
  }
  // Make a child-parent swap and MaxBubbleUp.
  if (node->parent()->score() < node->score()) {
    Swap(node->parent(), node);
    MaxBubbleUp(node->parent());
    return;
  }
  auto* grandparent = Grandparent(node);
  while (grandparent != NULL && grandparent->score() > node->score()) {
    Swap(grandparent, node);
    node = grandparent;
    grandparent = Grandparent(grandparent);
  }
}

static bool HasGrandchildren(MinMaxHeapNode* node) {
  return (node != NULL && node->left() != NULL && node->left()->left() != NULL);
}

static MinMaxHeapNode* CompareNodes(MinMaxHeapNode* left, MinMaxHeapNode* right,
                                    bool want_max) {
  if (right == NULL) {
    return left;
  }
  if (want_max) {
    return (left->score() >= right->score()) ? left : right;
  }
  return (left->score() <= right->score()) ? left : right;
}

static MinMaxHeapNode* Grandchild(MinMaxHeapNode* node,
                                  bool want_max_grandchildren) {
  auto* gc = node->left()->left();
  gc = CompareNodes(gc, node->left()->right(), want_max_grandchildren);
  gc = CompareNodes(gc, node->right()->left(), want_max_grandchildren);
  gc = CompareNodes(gc, node->right()->right(), want_max_grandchildren);
  return gc;
}

void MinMaxHeap::MaxSiftDown(MinMaxHeapNode* node) {
  if (node->left() != NULL) {
    if (node->left()->score() > node->score()) {
      Swap(node->left(), node);
    }
  }
  if (node->right() != NULL) {
    if (node->right()->score() > node->score()) {
      Swap(node->right(), node);
    }
  }
  if (HasGrandchildren(node)) {
    auto* max_grandchild = Grandchild(node, true);
    if (max_grandchild->score() > node->score()) {
      Swap(max_grandchild, node);
      MaxSiftDown(max_grandchild);
    }
  }
}

void MinMaxHeap::MinSiftDown(MinMaxHeapNode* node) {
  if (node->left() != NULL) {
    if (node->left()->score() < node->score()) {
      Swap(node->left(), node);
    }
  }
  if (node->right() != NULL) {
    if (node->right()->score() < node->score()) {
      Swap(node->right(), node);
    }
  }
  if (HasGrandchildren(node)) {
    auto* min_grandchild = Grandchild(node, false);
    if (min_grandchild->score() < node->score()) {
      Swap(min_grandchild, node);
      MinSiftDown(min_grandchild);
    }
  }
}

MinMaxHeapNode* MinMaxHeap::GetNode(unsigned node_id) const {
  if (node_id <= 1) {
    return root_.get();
  }
  unsigned bits = log2(node_id);
  unsigned mask = 1 << (bits - 1);
  auto* node = root_.get();
  // std::cout << "Looking for node " << node_id << std::endl;
  while (mask) {
    node = (node_id & mask) ? node->right() : node->left();
    if (node_id & mask) {
      // std::cout << "Going right to " << node->score() << std::endl;
    } else {
      // std::cout << "Going left to " << node->score() << std::endl;
    }
    mask >>= 1;
  }
  return node;
}

}  // namespce branch_and_bound
}  // namespace bayes
