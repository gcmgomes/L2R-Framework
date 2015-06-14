#include <cmath>
#include "k2.h"

namespace bayes {

K2::K2(::base::Dataset* dataset, unsigned bin_count)
    : dataset_(dataset), bin_count_(bin_count){};

// Determines to which bin |val| belongs.
static unsigned Bin(double val, unsigned bin_count) {
  return floor(val * (double)bin_count);
}

static void FillLogFact(unsigned max_log,
                        std::unordered_map<unsigned, double>& fact) {
  fact[0] = 0;
  unsigned i = 1;
  while (i <= max_log) {
    fact[i] = fact[i - 1] + log((double)i);
    i++;
  }
}

void K2::Init(unsigned max_log) {
  FillLogFact(max_log, log_fact_);
  auto query = dataset_->begin();
  while (query != dataset_->end()) {
    auto doc = query->begin();
    while (doc != query->end()) {
      auto feature = doc->begin();
      while (feature != doc->end()) {
        feature->second = Bin(feature->second, bin_count_);
        ++feature;
      }
      ++doc;
    }
    ++query;
  }
}

void K2::MakeParentStr(unsigned feature, const ::base::Document& d,
                       std::string& str) {
  str.clear();
  auto i = graph_[feature].begin();
  while (i != graph_[feature].end()) {
    char num[128];
    unsigned val = (unsigned)d.vector().at(*i);
    sprintf(num, "%u ", val);
    str += num;
    ++i;
  }
}

static unsigned Get_r_i(unsigned feature,
                        const std::unique_ptr<::base::Dataset>& dataset) {
  std::unordered_set<unsigned> size;
  auto query = dataset->begin();
  while (query != dataset->end()) {
    auto document = query->begin();
    while (document != query->end()) {
      unsigned value = (unsigned)document->vector().at(feature);
      size.insert(value);
      ++document;
    }
    ++query;
  }
  return size.size();
}

// We are computing log(g) = \sum_j [log((r_i-1)!) - log((N_ij + r_i - 1)!) +
// \sum_k log(N_ijk!)].
double K2::g(
    unsigned r_i,
    const std::unordered_map<
        std::string,
        std::pair<unsigned, std::unordered_map<unsigned, unsigned>>>& N_ijk) {
  auto j = N_ijk.begin();
  double sum = 0;
  while (j != N_ijk.end()) {
    unsigned N_ij = j->second.first;
    sum += log_fact_.at(r_i - 1) - log_fact_.at(N_ij + r_i - 1);
    auto k = j->second.second.begin();
    while (k != j->second.second.end()) {
      sum += log_fact_.at(k->second);
      ++k;
    }
    ++j;
  }
  return sum;
}

void K2::GetN_ijk(
    unsigned feature,
    std::unordered_map<
        std::string,
        std::pair<unsigned, std::unordered_map<unsigned, unsigned>>>& N_ijk) {
  auto query = dataset_->begin();
  while (query != dataset_->end()) {
    auto document = query->begin();
    while (document != query->end()) {
      std::string entry = "";
      MakeParentStr(feature, *document, entry);
      unsigned value = (unsigned)document->vector().at(feature);
      if (!N_ijk.count(entry)) {
        N_ijk[entry].first = 0;
      }
      N_ijk[entry].first++;
      N_ijk[entry].second[value]++;
      ++document;
    }
    ++query;
  }
}

double K2::Initial_g(unsigned feature) {
  unsigned r_i = Get_r_i(feature, dataset_);
  std::unordered_map<
      std::string, std::pair<unsigned, std::unordered_map<unsigned, unsigned>>>
      N_ijk;
  GetN_ijk(feature, N_ijk);
  double value = g(r_i, N_ijk);
  return value;
}

std::pair<unsigned, double> K2::MaxAddition(
    unsigned feature, std::unordered_set<unsigned> candidates) {
  unsigned r_i = Get_r_i(feature, dataset_);
  auto node = candidates.begin();
  std::pair<unsigned, double> ret_val = std::make_pair(0, -1000000.0);
  while (node != candidates.end()) {
    graph_[feature].insert(*node);
    std::unordered_map<
        std::string,
        std::pair<unsigned, std::unordered_map<unsigned, unsigned>>> N_ijk;
    GetN_ijk(feature, N_ijk);
    double value = g(r_i, N_ijk);
    if (ret_val.second < value) {
      ret_val.first = *node;
      ret_val.second = value;
    }
    graph_[feature].erase(*node);
    ++node;
  }
  return ret_val;
}

void K2::BuildNetwork(unsigned max_parents,
                      std::vector<unsigned> feature_order) {
  auto feature = feature_order.begin();
  graph_[*feature] = std::set<unsigned>();
  feature++;
  while (feature != feature_order.end()) {
    graph_[*feature] = std::set<unsigned>();  // pi = empty.
    double p_old = Initial_g(*feature);       // p_old = g(i, pi).

    std::unordered_set<unsigned> candidates(feature_order.begin(), feature);

    unsigned parent_count = 0;
    bool ok_to_proceed = true;
    while (ok_to_proceed && parent_count < max_parents && !candidates.empty()) {
      std::pair<unsigned, double> max_addition =
          MaxAddition(*feature, candidates);
      std::cout << "feature = " << *feature << " z = " << max_addition.first
                << " P_new = " << max_addition.second << " P_old = " << p_old
                << std::endl;
      if (max_addition.second > p_old) {
        p_old = max_addition.second;                  // p_old = p_new.
        graph_[*feature].insert(max_addition.first);  // pi = pi U z
        candidates.erase(max_addition.first);
        parent_count++;
      } else {
        ok_to_proceed = false;
      }
    }
    ++feature;
  }
}

void K2::PrintNetwork() {
  auto feature = graph_.begin();
  while(feature != graph_.end()) {
    auto parent = feature->second.begin();
    std::cout << "Feature: " << feature->first << " Parents:";
    while(parent != feature->second.end()) {
      if(parent != feature->second.begin()) {
        std::cout << ',';
      }
      std::cout << ' ' << *parent;
      ++parent;
    }
    std::cout << std::endl;
    ++feature;
  }
}

}  // namespace bayes
