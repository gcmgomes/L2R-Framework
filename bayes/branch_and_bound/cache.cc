#include "cache.h"
#include <cmath>
#include <limits>
#include <queue>
#include <sstream>
#include <unordered_set>

namespace bayes {

namespace branch_and_bound {

CacheEntry::CacheEntry() : CacheEntry(0, 0){};

CacheEntry::CacheEntry(long double log_likelihood,
                       unsigned long long free_parameters)
    : log_likelihood_(log_likelihood), free_parameters_(free_parameters){};

std::string CacheEntry::ToString() const {
  std::stringstream str;
  str << "LogLikelihood: " << log_likelihood_
      << " FreeParameters: " << free_parameters_;
  return str.str();
}

std::string CacheEntry::LogString() const {
  std::stringstream str;
  str << log_likelihood_ << " " << free_parameters_;
  return str.str();
}

Cache::Cache() : Cache(1.0, 1000) {
}

Cache::Cache(long double w, size_t maximum_cache_size)
    : w_(w), maximum_cache_size_(maximum_cache_size) {
}

void Cache::Insert(const Bitset& bitset, const CacheEntry& entry) {
  cache_[bitset] = entry;
  if (cache_.size() >= maximum_cache_size_) {
    WriteToRepository();
  }
}

bool Cache::BestCompliantEntry(const Bitset& prohibited_bits,
                               const Bitset& mandatory_bits,
                               Bitset& resulting_bits) const {
  Bitset best_complying_entry(prohibited_bits.bit_count());
  long double best_score = std::numeric_limits<int>::min();
  auto entry = cache_.cbegin();
  while (entry != cache_.cend()) {
    if (best_score < entry->second.score(w_) &&
        !prohibited_bits.BitwiseLogicalAnd(entry->first) &&
        entry->first.Contains(mandatory_bits)) {
      best_score = entry->second.score(w_);
      best_complying_entry = entry->first;
    }
    ++entry;
  }
  if (best_score == std::numeric_limits<int>::min()) {
    return false;
  }
  resulting_bits = best_complying_entry;
  return true;
}

void Cache::OpenRepository(const std::string& file_path,
                           std::ios_base::openmode open_mode) {
  if (repository_.is_open()) {
    repository_.close();
  }
  repository_.open(file_path, open_mode);
  if (open_mode == std::fstream::out) {
    repository_ << w_ << std::endl;
  }
}

void Cache::Flush() {
  WriteToRepository();
}

void Cache::InitializeCaches(const std::string& directory_root_path,
                             const std::vector<Instance>& instances,
                             std::vector<Cache>& caches,
                             size_t maximum_cache_size, Criterion criterion) {
  std::string path = directory_root_path;
  if (path.back() != '/') {
    path += '/';
  }
  long double w = 1;
  if (criterion == Criterion::MINIMUM_DESCRIPTION_LEGNTH) {
    w = log2(instances.size()) / 2.0;
  }
  unsigned variable_id = 0;
  while (variable_id < instances.at(0).values().size()) {
    std::stringstream str;
    str << path;
    str << "cache" << variable_id << ".txt";
    caches.emplace_back(w, maximum_cache_size);
    caches[variable_id].OpenRepository(str.str(), std::fstream::out);
    ++variable_id;
  }
}

void Cache::LoadCaches(const std::string& directory_root_path,
                       unsigned variable_count, std::vector<Cache>& caches) {
  std::string path = directory_root_path;
  if (path.back() != '/') {
    path += '/';
  }
  unsigned variable_id = 0;
  while (variable_id < variable_count) {
    std::stringstream str;
    str << path;
    str << "cache" << variable_id << ".txt";
    if (caches.size() <= variable_id) {
      caches.emplace_back(1000000, std::numeric_limits<size_t>::max());
    }
    caches[variable_id].OpenRepository(str.str(), std::fstream::in);
    caches[variable_id].LoadFromRepository();
    variable_id++;
  }
}

std::string Cache::ToString() const {
  if (cache_.empty()) {
    return std::string();
  }
  std::stringstream str_stream;
  str_stream.precision(6);
  str_stream << cache_.size() << std::endl;
  auto it = cache_.cbegin();
  while (it != cache_.cend()) {
    auto high_bits = it->first.high_bits();
    str_stream << std::fixed << it->second.score(w_);
    str_stream << " " << high_bits.size();
    auto bit_it = high_bits.cbegin();
    while (bit_it != high_bits.cend()) {
      str_stream << " " << *bit_it;
      ++bit_it;
    }
    str_stream << std::endl;
    ++it;
  }
  return str_stream.str();
}

void Cache::WriteToRepository() {
  if (!repository_.is_open()) {
    return;
  }
  auto it = cache_.cbegin();
  while (it != cache_.cend()) {
    repository_ << it->first.bit_string() << " " << it->second.LogString()
                << std::endl;
    ++it;
  }
  cache_.clear();
}

void Cache::LoadFromRepository() {
  repository_ >> w_;
  while (!repository_.eof()) {
    std::string bit_string;
    long double log_likelihood = 0;
    unsigned long long free_parameters = 0;
    repository_ >> bit_string >> log_likelihood >> free_parameters;
    if (repository_.eof()) {
      break;
    }
    Bitset bitset = Bitset::FromBitString(bit_string);
    CacheEntry entry(log_likelihood, free_parameters);
    Insert(bitset, entry);
  }
}

}  // namespce branch_and_bound
}  // namespace bayes
