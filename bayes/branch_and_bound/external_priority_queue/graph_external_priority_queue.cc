#include <algorithm>
#include <limits>
#include <sstream>
#include "graph_external_priority_queue.h"

namespace bayes {
namespace branch_and_bound {

GraphExternalPriorityQueue::GraphExternalPriorityQueue(
    size_t min_maximum_size, size_t max_maximum_size) {
  min_maximum_size_ = min_maximum_size;
  max_maximum_size_ = max_maximum_size;
  external_queue_.reset(new GraphExternalQueue("/var/tmp/queue.txt", 1000));
}

const Graph& GraphExternalPriorityQueue::min() const {
  if (!minima_.empty()) {
    return minima_.min();
  }
  return maxima_.min();
}

const Graph& GraphExternalPriorityQueue::max() const {
  if (maxima_.empty() && !minima_.empty()) {
    return minima_.max();
  }
  return maxima_.max();
}

void GraphExternalPriorityQueue::pop_min() {
  if (!minima_.empty()) {
    minima_.pop_min();
  } else if (memory_size() == total_size()) {  // |minima_| can only be empty if
    // every element in our outside of
    // memory is held by |maxima_|.
    maxima_.pop_min();
  }
  if ((minima_.empty() || maxima_.empty()) && total_size() > memory_size()) {
    Fetch();
  }
}

void GraphExternalPriorityQueue::pop_max() {
  if (!maxima_.empty()) {
    maxima_.pop_max();
  } else if (memory_size() == total_size()) {  // |minima_| can only be empty if
    // every element in our outside of
    // memory is held by |maxima_|.
    minima_.pop_max();
  }
  if ((minima_.empty() || maxima_.empty()) && total_size() > memory_size()) {
    Fetch();
  }
}

void GraphExternalPriorityQueue::push(const Graph& key) {
  bool min_range = IsInRange(key, false);
  bool max_range = IsInRange(key, true);
  if (min_range) {
    minima_.push(key);
    if (minima_.size() > min_maximum_size_) {
      Graph popped = minima_.max();
      minima_.pop_max();
      this->push(popped);
    }
  } else if (max_range) {
    maxima_.push(key);
    if (maxima_.size() > max_maximum_size_) {
      Graph popped = maxima_.min();
      maxima_.pop_min();
      this->push(popped);
    }
  } else {
    if (max_memory_size() <= total_size()) {
      Write(key);
    } else if (minima_.size() < min_maximum_size_) {
      minima_.push(key);
    } else {
      maxima_.push(key);
    }
  }
}

bool GraphExternalPriorityQueue::IsInRange(const Graph& key,
                                           bool check_maxima) {
  if (check_maxima) {
    if (maxima_.empty()) {
      return false;
    } else {
      return maxima_.min().score() < key.score();
    }
  }
  if (minima_.empty()) {
    return false;
  }
  return key.score() < minima_.max().score();
}

void GraphExternalPriorityQueue::Print() {
  std::cerr << "Minima: " << std::endl;
  minima_.Print();
  std::cerr << "Maxima: " << std::endl;
  maxima_.Print();
  std::cerr << std::endl;
}

void GraphExternalPriorityQueue::Write(const Graph& key) {
  external_queue_->push(key);
}

void GraphExternalPriorityQueue::Fetch() {
  std::string new_path = external_queue_->file_path();
  new_path += '+';
  std::unique_ptr<GraphExternalQueue> current_queue;
  current_queue.reset(external_queue_.release());
  external_queue_.reset(new GraphExternalQueue(new_path, 1000));
  while (!current_queue->empty()) {
    this->push(current_queue->front());
    current_queue->pop();
  }
  current_queue->Clear();
}

}  // namespce branch_and_bound
}  // namespace bayes
