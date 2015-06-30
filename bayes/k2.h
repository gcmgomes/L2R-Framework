#ifndef _RI_2015_1_BAYES_SMOTE_H_
#define _RI_2015_1_BAYES_SMOTE_H_

#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include "../base/dataset.h"

namespace bayes {

class DistributionEntry {
 public:
  DistributionEntry(const std::string& index, double prob)
      : index_(index), prob_(prob){};

  const std::string& index() const {
    return index_;
  }

  double prob() const {
    return prob_;
  }

  bool operator<(const DistributionEntry& e) const {
    return index() < e.index();
  }

 private:
  std::string index_;
  double prob_;
};

class DistributionMap {
 public:
  DistributionMap() : DistributionMap(0){};
  DistributionMap(unsigned variable_value) : variable_value_(variable_value){};

  unsigned variable_value() const {
    return variable_value_;
  }

  std::set<DistributionEntry>& entries() {
    return entries_;
  }

  bool operator<(const DistributionMap& m) const {
    return variable_value() < m.variable_value();
  }

 private:
  unsigned variable_value_;
  std::set<DistributionEntry> entries_;
};

class K2 {
 public:
  // Takes ownership if |dataset|. |bin_count| is the amount of bins used to
  // discretize the dataset. |use_label| indicates that the document's label
  // will be treated as a feature.
  K2(::base::Dataset* dataset, unsigned bin_count, bool use_label);

  const std::map<unsigned, std::set<unsigned>>& graph() const {
    return graph_;
  };

  std::map<unsigned, std::set<unsigned>>& mutable_graph() {
    return graph_;
  };

  const std::unique_ptr<::base::Dataset>& dataset() const {
    return dataset_;
  };

  std::unique_ptr<::base::Dataset>& mutable_dataset() {
    return dataset_;
  };

  // Initializes the needed structures. |max_log| controls the maximum log
  // factorial computed.
  void Init(unsigned max_log);

  // Builds the network.
  void BuildNetwork(unsigned max_parents, std::vector<unsigned> feature_order);

  // Computes the CPTs.
  void BuildCpts();

  // Prints the parentage information.
  void PrintNetwork();

 private:
  // Fills |N_ijk| with the mapping needed for the probability computation.
  void GetN_ijk(
      unsigned feature,
      std::unordered_map<
          std::string,
          std::pair<unsigned, std::unordered_map<unsigned, unsigned>>>& N_ijk);

  // Returns the pair (feature, P_new) that maximizes P_new.
  std::pair<unsigned, double> MaxAddition(
      unsigned feature, std::unordered_set<unsigned> candidates);

  // Create the string used to index N_ijk.
  void MakeParentStr(unsigned feature, const ::base::Document& d,
                     std::string& str);

  // Returns the value of g when there is no possible parent.
  double Initial_g(unsigned feature);

  // Returns the score of the added edge.
  double g(
      unsigned r_i,
      const std::unordered_map<
          std::string,
          std::pair<unsigned, std::unordered_map<unsigned, unsigned>>>& N_ijk);

  // Object containing the data.
  std::unique_ptr<::base::Dataset> dataset_;

  // The amount of bins used to discretize the dataset.
  unsigned bin_count_;

  // If true, the label will be used as the last feature in the order
  // construction.
  bool use_label_;

  // The graph containing parentage information. That is, the contents of
  // |graph_[i]| is the set of parents of node |i|.
  std::map<unsigned, std::set<unsigned>> graph_;

  // The CPTs mapping variable_id -> matrix_entries.
  std::map<unsigned, std::map<unsigned, DistributionMap>> graph_distribution_;

  // Structure storing the result of log_fact_[i] = log(i!).
  std::unordered_map<unsigned, double> log_fact_;
};

}  // namespace bayes

#endif
