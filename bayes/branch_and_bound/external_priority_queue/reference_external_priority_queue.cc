#include <algorithm>
#include <limits>
#include <sstream>
#include "reference_external_priority_queue.h"

namespace bayes {
namespace branch_and_bound {

ReferenceExternalPriorityQueue::ReferenceExternalPriorityQueue(
    size_t min_maximum_size, size_t max_maximum_size) {
  min_maximum_size_ = min_maximum_size;
  max_maximum_size_ = max_maximum_size;
  bounding_score_ = std::numeric_limits<long double>::min();
  external_queue_.reset(new ReferenceExternalQueue("/var/tmp/queue.txt"));
}

const Graph& ReferenceExternalPriorityQueue::min() const {
  if (!minima_.empty()) {
    return minima_.min();
  }
  return maxima_.min();
}

const Graph& ReferenceExternalPriorityQueue::max() const {
  if (maxima_.empty()) {
    return minima_.max();
  }
  return maxima_.max();
}

void ReferenceExternalPriorityQueue::pop_min() {
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

void ReferenceExternalPriorityQueue::pop_max() {
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

void ReferenceExternalPriorityQueue::push(const Graph& key) {
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

bool ReferenceExternalPriorityQueue::IsInRange(const Graph& key,
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

void ReferenceExternalPriorityQueue::Write(const Graph& key) {
  external_queue_->push(key);
}

void ReferenceExternalPriorityQueue::Fetch() {
  std::cerr << std::endl
            << "Fetching" << std::endl;
  // std::cerr << "Before status: Minima " << minima_size()
  //          << " Maxima " << maxima_size()
  //          << " External " << external_queue_->size() << std::endl;
  while (minima_size() < min_maximum_size() && !external_queue_->empty()) {
    Graph min = external_queue_->min();
    //std::cerr << "(MIN " << min.score() << ") Min size: " << minima_size() << " Max size: " << maxima_size()
    //          << std::endl;
    if (min.score() > bounding_score_) {
      minima_.push(min);
    }
    external_queue_->pop_min();
  }
  while (maxima_size() < max_maximum_size() && !external_queue_->empty()) {
    Graph max = external_queue_->max();
    //std::cerr << "(MAX) Min size: " << minima_size() << " Max size: " << maxima_size()
    //          << std::endl;
    if (max.score() > bounding_score_) {
      maxima_.push(max);
    }
    external_queue_->pop_max();
  }
  // std::cerr << "After status: Minima " << minima_size()
  //          << " Maxima " << maxima_size()
  //          << " External " << external_size()
  //          << " Prev size " << external_queue_->size() << std::endl;
}

}  // namespce branch_and_bound
}  // namespace bayes
