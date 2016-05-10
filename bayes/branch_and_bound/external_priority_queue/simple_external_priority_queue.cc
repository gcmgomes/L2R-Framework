#include <algorithm>
#include <limits>
#include <sstream>
#include "simple_external_priority_queue.h"

namespace bayes {
namespace branch_and_bound {

SimpleExternalPriorityQueue::SimpleExternalPriorityQueue(size_t min_maximum_size,
                                             size_t max_maximum_size) {
  min_maximum_size_ = min_maximum_size;
  max_maximum_size_ = max_maximum_size;
  external_queue_.reset(new SimpleExternalQueue(
      "/var/tmp/queue.txt", 1000));
}

int SimpleExternalPriorityQueue::min() const {
  if (!minima_.empty()) {
    return minima_.min();
  }
  return maxima_.min();
}

int SimpleExternalPriorityQueue::max() const {
  if (maxima_.empty() && !minima_.empty()) {
    return minima_.max();
  }
  return maxima_.max();
}

void SimpleExternalPriorityQueue::pop_min() {
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

void SimpleExternalPriorityQueue::pop_max() {
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

void SimpleExternalPriorityQueue::push(const int& key) {
  bool min_range = IsInRange(key, false);
  bool max_range = IsInRange(key, true);
  if (min_range) {
    minima_.push(key);
    if (minima_.size() > min_maximum_size_) {
      int popped = minima_.max();
      minima_.pop_max();
      this->push(popped);
    }
  } else if (max_range) {
    maxima_.push(key);
    if (maxima_.size() > max_maximum_size_) {
      int popped = maxima_.min();
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

bool SimpleExternalPriorityQueue::IsInRange(const int& key, bool check_maxima) {
  if (check_maxima) {
    if (maxima_.empty()) {
      return false;
    } else {
      return maxima_.min() < key;
    }
  }
  if (minima_.empty()) {
    return false;
  }
  return key < minima_.max();
}

void SimpleExternalPriorityQueue::Print() {
  std::cout << "Minima: " << std::endl;
  minima_.Print();
  std::cout << "Maxima: " << std::endl;
  maxima_.Print();
  std::cout << std::endl;
}

void SimpleExternalPriorityQueue::Write(const int& key) {
  external_queue_->push(key);
}

void SimpleExternalPriorityQueue::Fetch() {
  std::string new_path = external_queue_->file_path();
  new_path += '+';
  std::unique_ptr<SimpleExternalQueue> current_queue;
  current_queue.reset(external_queue_.release());
  external_queue_.reset(new SimpleExternalQueue(new_path, 1000));
  while(!current_queue->empty()) {
    int front = current_queue->front();
    current_queue->pop();
    this->push(front);
  }
  current_queue->Clear();
}

}  // namespce branch_and_bound
}  // namespace bayes
