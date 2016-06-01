// This heap structure will act as a double ended priority queue for the branch
// and bound proceedure. Experiments showed that the standard STL multiset is
// a bit slow and not really designed for this task. More
// over, std::priority_queue does not support both top/max and bottom/min
// operations.
// Nodes on even levels of the heap are MIN nodes, the others are MAX nodes.
#ifndef _L2RF_UTIL_DATA_STRUCTURES_MIN_MAX_HEAP_H_
#define _L2RF_UTIL_DATA_STRUCTURES_MIN_MAX_HEAP_H_

#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>
#include "min_max_heap_node.h"

namespace util {
namespace data_structures {

template <class T>
class MinMaxHeap {
 public:
  MinMaxHeap() :size_(0) {};

  size_t size() const {
    return size_;
  }

  void push(const T& key);

  const T& max() const;

  const T& min() const;

  void pop_max();

  void pop_min();

  bool empty() const;

 private:
  void Swap(MinMaxHeapNode<T>* a, MinMaxHeapNode<T>* b);

  void MinMaxHeapify(MinMaxHeapNode<T>* node);

  // Node is always a max level node.
  void MaxBubbleUp(MinMaxHeapNode<T>* node);

  // Node is always a min level node.
  void MinBubbleUp(MinMaxHeapNode<T>* node);

  // Node is always a max level node.
  void MaxSiftDown(MinMaxHeapNode<T>* node);

  // Node is always a min level node.
  void MinSiftDown(MinMaxHeapNode<T>* node);

  MinMaxHeapNode<T>* GetNode(unsigned node_id) const;

  size_t size_;
  std::unique_ptr<MinMaxHeapNode<T>> root_;
};

template <class T>
void MinMaxHeap<T>::Swap(MinMaxHeapNode<T>* a, MinMaxHeapNode<T>* b) {
  T tmp = a->key();
  a->mutable_key() = b->mutable_key();
  b->mutable_key() = tmp;
}

template <class T>
void MinMaxHeap<T>::push(const T& key) {
  size_++;
  if (size_ == 1) {
    root_.reset(new MinMaxHeapNode<T>(key, NULL));
    return;
  }
  auto* node = GetNode(size_ >> 1);
  if (node->left() == NULL) {
    node->mutable_left().reset(new MinMaxHeapNode<T>(key, node));
    node = node->left();
  } else {
    node->mutable_right().reset(new MinMaxHeapNode<T>(key, node));
    node = node->right();
  }
  MinMaxHeapify(node);
}

template <class T>
const T& MinMaxHeap<T>::max() const {
  if (size_ <= 1) {
    return root_->key();
  } else if (size_ == 2) {
    return root_->left()->key();
  } else if (*root_->left() > *root_->right()) {
    return root_->left()->key();
  }
  return root_->right()->key();
}

template <class T>
const T& MinMaxHeap<T>::min() const {
  return root_->key();
}

template <class T>
void MinMaxHeap<T>::pop_max() {
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
  auto* maximum =
      (*root_->left() > *root_->right()) ? root_->left() : root_->right();
  auto* penultimate_node = GetNode(size_ >> 1);
  if (size_ & 1) {
    Swap(maximum, penultimate_node->right());
    penultimate_node->mutable_right().reset();
  } else {
    Swap(maximum, penultimate_node->left());
    penultimate_node->mutable_left().reset();
  }
  size_--;
  if (size_ > 2) {
    MaxSiftDown(maximum);
  }
}

template <class T>
void MinMaxHeap<T>::pop_min() {
  if (size_ <= 1) {
    size_ = 0;
    root_.reset();
    return;
  }
  auto* penultimate_node = GetNode(size_ >> 1);
  if (size_ & 1) {
    // std::cout << "Swapping " << root_ << " with "
    //           << penultimate_node->right() << std::endl;
    Swap(root_.get(), penultimate_node->right());
    penultimate_node->mutable_right().reset();
  } else {
    // std::cout << "Swapping " << root_ << " with "
    //           << penultimate_node->left() << std::endl;
    Swap(root_.get(), penultimate_node->left());
    penultimate_node->mutable_left().reset();
  }
  size_--;
  MinSiftDown(root_.get());
}

template <class T>
bool MinMaxHeap<T>::empty() const {
  return size_ == 0;
}

template <class T>
void MinMaxHeap<T>::MinMaxHeapify(MinMaxHeapNode<T>* node) {
  bool is_max_node = ((unsigned)log2(size_)) % 2;
  if (is_max_node) {
    MaxBubbleUp(node);
  } else {
    MinBubbleUp(node);
  }
}

template <class T>
void MinMaxHeap<T>::MaxBubbleUp(MinMaxHeapNode<T>* node) {
  if (node->parent() == NULL) {
    return;
  }
  // Make a child-parent swap and MinBubbleUp.
  if (*node->parent() > *node) {
    Swap(node->parent(), node);
    MinBubbleUp(node->parent());
    return;
  }
  auto* grandparent = (node == NULL) ? NULL : node->Grandparent();
  while (grandparent != NULL && *grandparent < *node) {
    Swap(grandparent, node);
    node = grandparent;
    grandparent = grandparent->Grandparent();
  }
}

template <class T>
void MinMaxHeap<T>::MinBubbleUp(MinMaxHeapNode<T>* node) {
  if (node->parent() == NULL) {
    return;
  }
  // Make a child-parent swap and MaxBubbleUp.
  if (*node->parent() < *node) {
    Swap(node->parent(), node);
    MaxBubbleUp(node->parent());
    return;
  }
  auto* grandparent = (node == NULL) ? NULL : node->Grandparent();
  while (grandparent != NULL && *grandparent > *node) {
    Swap(grandparent, node);
    node = grandparent;
    grandparent = grandparent->Grandparent();
  }
}

template <class T>
void MinMaxHeap<T>::MaxSiftDown(MinMaxHeapNode<T>* node) {
  if (node->left() != NULL) {
    if (*node->left() > *node) {
      Swap(node->left(), node);
    }
  }
  if (node->right() != NULL) {
    if (*node->right() > *node) {
      Swap(node->right(), node);
    }
  }
  if (node != NULL && node->HasGrandchildren()) {
    auto* max_grandchild = node->MaxGrandchild();
    if (*max_grandchild > *node) {
      Swap(max_grandchild, node);
      MaxSiftDown(max_grandchild);
    }
  }
}

template <class T>
void MinMaxHeap<T>::MinSiftDown(MinMaxHeapNode<T>* node) {
  if (node->left() != NULL) {
    if (node->left() < node) {
      Swap(node->left(), node);
    }
  }
  if (node->right() != NULL) {
    if (*node->right() < *node) {
      Swap(node->right(), node);
    }
  }
  if (node != NULL && node->HasGrandchildren()) {
    auto* min_grandchild = node->MinGrandchild();
    if (*min_grandchild < *node) {
      Swap(min_grandchild, node);
      MinSiftDown(min_grandchild);
    }
  }
}

template <class T>
MinMaxHeapNode<T>* MinMaxHeap<T>::GetNode(unsigned node_id) const {
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
      // std::cout << "Going right to " << node << std::endl;
    } else {
      // std::cout << "Going left to " << node << std::endl;
    }
    mask >>= 1;
  }
  return node;
}

}  // namespce data_structures
}  // namespace util

#endif
