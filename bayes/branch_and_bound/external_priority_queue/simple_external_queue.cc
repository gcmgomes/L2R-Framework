#include <sstream>
#include "simple_external_queue.h"

namespace bayes {
namespace branch_and_bound {

SimpleExternalQueue::SimpleExternalQueue(const std::string& file_path,
                                         size_t queue_limit) {
  queue_limit_ = queue_limit;
  size_ = 0;
  file_path_ = file_path;
  tellg_ = tellp_ = 0;
  Initialize(file_path);
}

bool SimpleExternalQueue::empty() {
  return queue_.empty() &&
         (repository_.tellg() == std::ios::end || repository_.peek() == EOF);
}

const int& SimpleExternalQueue::front() const {
  return queue_.front();
}

void SimpleExternalQueue::push(const int& key) {
  if (queue_.size() < queue_limit_ && size_ == queue_.size()) {
    queue_.push(key);
  } else {
    if (tellp_ != repository_.tellp()) {
      repository_.seekp(tellp_);
    }
    repository_.clear();
    // std::cerr << "Writing to " << tellp_ << std::endl;
    repository_ << key << std::endl;
    tellp_ = repository_.tellp();
    repository_.flush();
  }
  size_++;
}

void SimpleExternalQueue::pop() {
  size_--;
  if (!queue_.empty()) {
    queue_.pop();
  }
  if (queue_.empty()) {
    FetchFromRepository();
  }
}

void SimpleExternalQueue::Initialize(const std::string& file_path) {
  OpenRepository(file_path);
}

void SimpleExternalQueue::OpenRepository(const std::string& file_path) {
  tellg_ = tellp_ = 0;
  if (repository_.is_open()) {
    repository_.close();
  }
  repository_.open(file_path,
                   std::fstream::in | std::fstream::out | std::fstream::trunc);
}

void SimpleExternalQueue::FetchFromRepository() {
  repository_.clear();
  repository_.sync();
  if (tellg_ != repository_.tellg()) {
    repository_.seekg(tellg_);
  }
  while (queue_.size() < queue_limit_) {
    int key = 0;
    repository_ >> key;
    if (repository_.peek() == EOF) {
      break;
    }
    // std::cerr << "Reading from " << tellg_ << std::endl;
    tellg_ = repository_.tellg();
    queue_.push(key);
  }
}

void SimpleExternalQueue::Clear() {
  remove(file_path_.c_str());
}

}  // namespce branch_and_bound
}  // namespace bayes
