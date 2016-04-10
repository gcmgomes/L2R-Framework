#include <cmath>
#include <queue>
#include <sstream>
#include <unordered_set>
#include "cache.h"

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

static void Subset(Bitset seed, std::unordered_set<Bitset>& subsets) {
  std::queue<Bitset> queue;
  queue.push(seed);
  while (!queue.empty()) {
    Bitset t = queue.front();
    queue.pop();
    auto it = t.high_bits().cbegin();
    while (it != t.high_bits().cend()) {
      Bitset sub = t;
      sub.Set(*it, false);
      if (!subsets.count(sub)) {
        subsets.insert(sub);
        queue.push(sub);
      }
      ++it;
    }
  }
}

bool Cache::IsConsistent() const {
  auto it = cache_.cbegin();
  while (it != cache_.cend()) {
    std::unordered_set<Bitset> subsets;
    Subset(it->first, subsets);
    auto jt = subsets.cbegin();
    while (jt != subsets.cend()) {
      if (!cache_.count(*jt)) {
        std::cerr << "INCONSISTENCY!" << std::endl
                  << "\tSubset: " << jt->ToString() << std::endl
                  << "\t Superset: " << it->first.ToString() << std::endl;
        return false;
      }
      ++jt;
    }
    ++it;
  }
  return true;
}

void Cache::OpenRepository(const std::string& file_path) {
  repository_.open(file_path, std::fstream::out);
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
    caches[variable_id].OpenRepository(str.str());
    ++variable_id;
  }
}

void Cache::Flush() {
  WriteToRepository();
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

}  // namespce branch_and_bound
}  // namespace bayes
