// This structure is an external double ended priority queue for ints. It will
// possibly be extended as a template class, but as demand hasn't risen yet, we
// will not. Until then, every instantiation will reimplement this data
// structure to fit it's needs.
#ifndef _L2RF_BAYES_BRANCH_AND_BOUND_SIMPLE_EXTERNAL_PRIORITY_QUEUE_H_
#define _L2RF_BAYES_BRANCH_AND_BOUND_SIMPLE_EXTERNAL_PRIORITY_QUEUE_H_

#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>
#include "simple_external_queue.h"
#include "simple_min_max_heap.h"

namespace bayes {
namespace branch_and_bound {

class SimpleExternalPriorityQueue {
 public:
  SimpleExternalPriorityQueue(size_t min_maximum_size, size_t max_maximum_size);

  size_t total_size() const {
    return minima_.size() + maxima_.size() + external_queue_->size();
  }

  size_t memory_size() const {
    return minima_.size() + maxima_.size();
  }

  size_t min_maximum_size() const {
    return min_maximum_size_;
  }

  size_t max_maximum_size() const {
    return max_maximum_size_;
  }

  size_t max_memory_size() const {
    return min_maximum_size_ + max_maximum_size_;
  }

  bool empty() {
    return total_size() == 0;
  }

  int min() const;
  int max() const;

  void pop_min();
  void pop_max();

  void push(const int& key);

  void Print();

 private:
  bool IsInRange(const int& key, bool check_maxima);

  void Write(const int& key);

  void Fetch();

  size_t min_maximum_size_;
  size_t max_maximum_size_;

  SimpleMinMaxHeap minima_;
  SimpleMinMaxHeap maxima_;

  std::unique_ptr<SimpleExternalQueue> external_queue_;
};

}  // namespce branch_and_bound
}  // namespace bayes

#endif
