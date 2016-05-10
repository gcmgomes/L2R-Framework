// This structure is an external double ended priority queue for ints. It will
// possibly be extended as a template class, but as demand hasn't risen yet, we
// will not. Until then, every instantiation will reimplement this data
// structure to fit it's needs.
#ifndef _L2RF_BAYES_BRANCH_AND_BOUND_GRAPH_EXTERNAL_PRIORITY_QUEUE_H_
#define _L2RF_BAYES_BRANCH_AND_BOUND_GRAPH_EXTERNAL_PRIORITY_QUEUE_H_

#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>
#include "graph_external_queue.h"
#include "graph_min_max_heap.h"

namespace bayes {
namespace branch_and_bound {

class GraphExternalPriorityQueue {
 public:
  GraphExternalPriorityQueue(size_t min_maximum_size, size_t max_maximum_size);

  size_t total_size() const {
    return minima_.size() + maxima_.size() + external_queue_->size();
  }

  bool empty() const {
    return !total_size();
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

  const Graph& min() const;
  const Graph& max() const;

  void pop_min();
  void pop_max();

  void push(const Graph& key);

  void Print();

 private:
  bool IsInRange(const Graph& key, bool check_maxima);

  void Write(const Graph& key);

  void Fetch();

  size_t min_maximum_size_;
  size_t max_maximum_size_;

  GraphMinMaxHeap minima_;
  GraphMinMaxHeap maxima_;

  std::unique_ptr<GraphExternalQueue> external_queue_;
};

}  // namespce branch_and_bound
}  // namespace bayes

#endif
