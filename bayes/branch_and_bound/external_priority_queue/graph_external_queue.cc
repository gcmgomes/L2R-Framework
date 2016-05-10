#include <sstream>
#include "graph_external_queue.h"

namespace bayes {
namespace branch_and_bound {

GraphExternalQueue::GraphExternalQueue(const std::string& file_path,
                                       size_t queue_limit) {
  queue_limit_ = queue_limit;
  size_ = 0;
  file_path_ = file_path;
  tellg_ = tellp_ = 0;
  Initialize(file_path);
}

bool GraphExternalQueue::empty() {
  return queue_.empty() &&
         (repository_.tellg() == std::ios::end || repository_.peek() == EOF);
}

const Graph& GraphExternalQueue::front() const {
  return queue_.front();
}

static std::string ToLogString(const Graph& key) {
  std::stringstream str;
  str << key.variables().size() << std::endl;
  str << key.score() << std::endl;
  auto var_it = key.variables().cbegin();
  while (var_it != key.variables().cend()) {
    str << var_it->variable_id() << " " << var_it->cache() << " "
        << var_it->external_queue() << " " << var_it->parent_set().bit_string()
        << std::endl;
    ++var_it;
  }
  unsigned edge_count = 0;
  auto it = key.h_matrix().cbegin();
  while (it != key.h_matrix().cend()) {
    edge_count += it->second.size();
    ++it;
  }
  str << edge_count << std::endl;
  it = key.h_matrix().cbegin();
  while (it != key.h_matrix().cend()) {
    auto jt = it->second.cbegin();
    while (jt != it->second.cend()) {
      str << it->first << " " << jt->first << " ";
      unsigned char c = (jt->second == ArcStatus::PRESENT) ? 1 : 0;
      str << c << std::endl;
      ++jt;
    }
    ++it;
  }
  return str.str();
}

static void FromLogString(std::fstream& repository, Graph& key) {
  long double score = 0;
  unsigned variable_count = 0;
  key.mutable_variables().clear();
  repository >> variable_count;
  repository >> score;
  if (repository.eof() || repository.peek() == EOF) {
    return;
  }
  while (variable_count) {
    std::string bit_string;
    unsigned variable_id = 0;
    unsigned long long cache_ptr = 0, external_queue_ptr = 0;
    repository >> variable_id >> cache_ptr >> external_queue_ptr >> bit_string;
    Cache* cache = (Cache*) cache_ptr;
    ExternalQueue* external_queue = (ExternalQueue*) external_queue_ptr;
    key.mutable_variables().emplace_back(variable_id, cache, external_queue,
                                         bit_string.size());
    key.mutable_variables().back().mutable_parent_set() =
        Bitset::FromBitString(bit_string);
    variable_count--;
  }
  unsigned edge_count = 0;
  repository >> edge_count;
  while (edge_count) {
    unsigned char u = 0, v = 0, status = 0;
    repository >> u >> v >> status;
    key.mutable_h_matrix()[u][v] =
        (status) ? ArcStatus::PRESENT : ArcStatus::PROHIBITED;
    edge_count--;
  }
}

void GraphExternalQueue::push(const Graph& key) {
  if (queue_.size() < queue_limit_ && size_ == queue_.size()) {
    queue_.push(key);
  } else {
    if (tellp_ != repository_.tellp()) {
      repository_.seekp(tellp_);
    }
    repository_.clear();
    repository_ << ToLogString(key) << std::endl;
    tellp_ = repository_.tellp();
    repository_.flush();
  }
  size_++;
}

void GraphExternalQueue::pop() {
  size_--;
  if (!queue_.empty()) {
    queue_.pop();
  }
  if (queue_.empty()) {
    FetchFromRepository();
  }
}

void GraphExternalQueue::Initialize(const std::string& file_path) {
  OpenRepository(file_path);
}

void GraphExternalQueue::OpenRepository(const std::string& file_path) {
  tellg_ = tellp_ = 0;
  if (repository_.is_open()) {
    repository_.close();
  }
  repository_.open(file_path,
                   std::fstream::in | std::fstream::out | std::fstream::trunc);
}

void GraphExternalQueue::FetchFromRepository() {
  repository_.clear();
  repository_.sync();
  if (tellg_ != repository_.tellg()) {
    repository_.seekg(tellg_);
  }
  while (queue_.size() < queue_limit_) {
    Graph key;
    FromLogString(repository_, key);
    if (repository_.peek() == EOF) {
      break;
    }
    tellg_ = repository_.tellg();
    queue_.push(key);
  }
}

void GraphExternalQueue::Clear() {
  remove(file_path_.c_str());
}

}  // namespce branch_and_bound
}  // namespace bayes
