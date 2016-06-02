#ifndef _L2RF_UTIL_DATA_STRUCTURES_MIN_MAX_HEAP_NODE_H_
#define _L2RF_UTIL_DATA_STRUCTURES_MIN_MAX_HEAP_NODE_H_

#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

namespace util {
namespace data_structures {

template <class T>
class MinMaxHeapNode {
 public:
  MinMaxHeapNode(const T& key, MinMaxHeapNode<T>* parent)
      : key_(key), parent_(parent){};

  const T& key() const {
    return key_;
  }

  T& mutable_key() {
    return key_;
  }

  MinMaxHeapNode<T>* parent() const {
    return parent_;
  }

  MinMaxHeapNode<T>* left() const {
    return left_.get();
  }

  std::unique_ptr<MinMaxHeapNode<T>>& mutable_left() {
    return left_;
  }

  MinMaxHeapNode<T>* right() const {
    return right_.get();
  }

  std::unique_ptr<MinMaxHeapNode<T>>& mutable_right() {
    return right_;
  }

  MinMaxHeapNode<T>* Grandparent() const;

  bool HasGrandchildren() const;

  MinMaxHeapNode<T>* MaxGrandchild() const;

  MinMaxHeapNode<T>* MinGrandchild() const;

  bool operator<(const MinMaxHeapNode<T>& node) {
    return this->key() < node.key();
  }

  bool operator>(const MinMaxHeapNode<T>& node) {
    return node.key() < this->key();
  }

  void Print(std::string padding) {
    std::cout << padding << key_ << std::endl;
    if (right_ == NULL) {
      std::cout << padding << "  ||" << std::endl;
    } else {
      right_->Print(padding + "  ");
    }
    if (left_ == NULL) {
      std::cout << padding << "  ||" << std::endl;
    } else {
      left_->Print(padding + "  ");
    }
  }

 private:
  T key_;

  MinMaxHeapNode<T>* parent_;

  std::unique_ptr<MinMaxHeapNode<T>> left_;

  std::unique_ptr<MinMaxHeapNode<T>> right_;
};

template <class T>
MinMaxHeapNode<T>* MinMaxHeapNode<T>::Grandparent() const {
  if (this->parent() != NULL && this->parent()->parent() != NULL) {
    return this->parent()->parent();
  }
  return NULL;
}

template <class T>
bool MinMaxHeapNode<T>::HasGrandchildren() const {
  return (this->left() != NULL && this->left()->left() != NULL);
}

template <class T>
MinMaxHeapNode<T>* MinMaxHeapNode<T>::MaxGrandchild() const {
  auto* gc = this->left()->left();
  gc = (this->left()->right() != NULL && *gc < *this->left()->right())
           ? this->left()->right()
           : gc;
  gc = (this->right()->left() != NULL && *gc < *this->right()->left())
           ? this->right()->left()
           : gc;
  gc = (this->right()->right() != NULL && *gc < *this->right()->right())
           ? this->right()->right()
           : gc;
  return gc;
}

template <class T>
MinMaxHeapNode<T>* MinMaxHeapNode<T>::MinGrandchild() const {
  auto* gc = this->left()->left();
  gc = (this->left()->right() == NULL || *gc < *this->left()->right())
           ? gc
           : this->left()->right();
  gc = (this->right()->left() == NULL || *gc < *this->right()->left())
           ? gc
           : this->right()->left();
  gc = (this->right()->right() == NULL || *gc < *this->right()->right())
           ? gc
           : this->right()->right();
  return gc;
}

}  // namespce data_structures
}  // namespace util

#endif
