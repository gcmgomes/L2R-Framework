#include "reference_external_queue.h"
#include <sstream>

namespace bayes {
namespace branch_and_bound {

ReferenceExternalQueue::ReferenceExternalQueue(const std::string& file_path) {
  file_path_ = file_path;
  tellp_ = 0;
  Initialize(file_path);
}

bool ReferenceExternalQueue::empty() {
  return !size();
}

Graph ReferenceExternalQueue::min() {
  // std::cerr << "MinReference Score:" << reference_heap_.min().score()
  //          << " Pos:" << reference_heap_.min().stream_pos() << std::endl;
  return FetchGraph(reference_heap_.min());
}

Graph ReferenceExternalQueue::max() {
  return FetchGraph(reference_heap_.max());
}

static std::string ToLogString(const Graph& key) {
  std::stringstream str;
  str << key.variables().size() << std::endl;
  str << key.score() << std::endl;
  auto var_it = key.variables().cbegin();
  while (var_it != key.variables().cend()) {
    unsigned long long cache_ptr = (unsigned long long)var_it->cache(),
                       eq_ptr = (unsigned long long)var_it->cp_table();
    str << var_it->variable_id() << " " << cache_ptr << " " << eq_ptr << " "
        << var_it->parent_set().bit_string() << std::endl;
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
      str << (unsigned)it->first << " " << (unsigned)jt->first << " ";
      unsigned c = (jt->second == ArcStatus::PRESENT) ? 1 : 0;
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
  key = Graph(score);
  if (repository.eof() || repository.peek() == EOF) {
    return;
  }
  // std::cerr << "Vars: " << variable_count << std::endl;
  // std::cerr << "Score: " << score << std::endl;
  // std::cerr << "Var structs" << std::endl;
  while (variable_count) {
    unsigned variable_id = 0;
    unsigned long long cache_ptr = 0, cp_table_ptr = 0;
    std::string bit_string;
    repository >> variable_id >> cache_ptr >> cp_table_ptr >> bit_string;
    Cache* cache = (Cache*)cache_ptr;
    inference::CPTable* cp_table = (inference::CPTable*)cp_table_ptr;
    key.mutable_variables().emplace_back(
        variable_id, Bitset::FromBitString(bit_string), cache, cp_table);
    // std::cerr << key.mutable_variables().back().variable_id()
    //          << " " << (unsigned long long)
    //          key.mutable_variables().back().cache()
    //          << " " << (unsigned long long)
    //          key.mutable_variables().back().cp_table()
    //          << " " <<
    //          key.mutable_variables().back().parent_set().bit_string()
    //          << std::endl;
    variable_count--;
  }
  unsigned edge_count = 0;
  repository >> edge_count;
  // std::cerr << edge_count << std::endl;
  // std::cerr << "Matrix" << std::endl;
  while (edge_count) {
    unsigned u = 0, v = 0, status = 0;
    repository >> u >> v >> status;
    // std::cerr << u << " " << v << " " << status << std::endl;
    key.mutable_h_matrix()[u][v] =
        (status) ? ArcStatus::PRESENT : ArcStatus::PROHIBITED;
    edge_count--;
  }
  // std::cerr << "Done" << std::endl;
}

void ReferenceExternalQueue::push(const Graph& key) {
  if (tellp_ != repository_.tellp()) {
    repository_.seekp(tellp_);
  }
  repository_.clear();
  repository_ << ToLogString(key);
  ExternalGraphReference reference(key, tellp_);
  // std::cerr << "Wrote (Score: " << reference.score()
  //          << ", Pos: " << reference.stream_pos() << ")" << std::endl;
  reference_heap_.push(reference);
  tellp_ = repository_.tellp();
  repository_.flush();
}

void ReferenceExternalQueue::pop_min() {
  reference_heap_.pop_min();
}

void ReferenceExternalQueue::pop_max() {
  reference_heap_.pop_max();
}

void ReferenceExternalQueue::Initialize(const std::string& file_path) {
  OpenRepository(file_path);
}

void ReferenceExternalQueue::OpenRepository(const std::string& file_path) {
  tellp_ = 0;
  if (repository_.is_open()) {
    repository_.close();
  }
  repository_.open(file_path,
                   std::fstream::in | std::fstream::out | std::fstream::trunc);
}

Graph ReferenceExternalQueue::FetchGraph(
    const ExternalGraphReference& reference) {
  repository_.clear();
  repository_.sync();
  if (reference.stream_pos() != repository_.tellg()) {
    repository_.seekg(reference.stream_pos());
  }
  Graph key;
  FromLogString(repository_, key);
  return key;
}

void ReferenceExternalQueue::Clear() {
  remove(file_path_.c_str());
}

}  // namespce branch_and_bound
}  // namespace bayes
