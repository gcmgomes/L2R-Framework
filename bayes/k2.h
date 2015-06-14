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

class K2 {
 public:
  // Takes ownership if |dataset|. |bin_count| is the amount of bins used to
  // discretize the dataset.
  K2(::base::Dataset* dataset, unsigned bin_count);

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

  // Builds the network // Builds the network.
  void BuildNetwork(unsigned max_parents, std::vector<unsigned> feature_order);

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

  unsigned bin_count_;

  // The graph containing parentage information. That is, the contents of
  // |graph_[i]| is the set of parents of node |i|.
  std::map<unsigned, std::set<unsigned>> graph_;

  // Structure storing the result of log_fact_[i] = log(i!).
  std::unordered_map<unsigned, double> log_fact_;
};

}  // namespace bayes

#endif
