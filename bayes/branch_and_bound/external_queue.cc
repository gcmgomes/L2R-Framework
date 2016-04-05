#include "external_queue.h"

namespace bayes {
namespace branch_and_bound {

ExternalQueue::ExternalQueue(const std::string& file_path, size_t queue_limit) {
  queue_limit_ = queue_limit;
  size_ = 0;
  tellg_ = tellp_ = 0;
  repository_.open(file_path,
                   std::fstream::in | std::fstream::out | std::fstream::trunc);
}

bool ExternalQueue::empty() {
  return queue_.empty() &&
         (repository_.tellg() == std::ios::end || repository_.peek() == EOF);
}

const Bitset& ExternalQueue::front() const {
  return queue_.front();
}

void ExternalQueue::push(const Bitset& bitset) {
  if (queue_.size() < queue_limit_ && size_ == queue_.size()) {
    queue_.push(bitset);
  } else {
    if (tellp_ != repository_.tellp()) {
      repository_.seekp(tellp_);
    }
    repository_.clear();
    //std::cerr << "Writing to " << tellp_ << std::endl;
    repository_ << bitset.bit_string() << std::endl;
    tellp_ = repository_.tellp();
    repository_.flush();
  }
  size_++;
}

void ExternalQueue::pop() {
  size_--;
  if (!queue_.empty()) {
    queue_.pop();
  }
  if (queue_.empty()) {
    FetchFromRepository();
  }
}

void ExternalQueue::FetchFromRepository() {
  repository_.clear();
  repository_.sync();
  if (tellg_ != repository_.tellg()) {
    repository_.seekg(tellg_);
  }
  while (queue_.size() < queue_limit_) {
    std::string bit_string;
    repository_ >> bit_string;
    if (repository_.peek() == EOF) {
      break;
    }
    // std::cerr << "Reading from " << tellg_ << std::endl;
    tellg_ = repository_.tellg();
    queue_.push(Bitset::FromBitString(bit_string));
  }
}

}  // namespce branch_and_bound
}  // namespace bayes
