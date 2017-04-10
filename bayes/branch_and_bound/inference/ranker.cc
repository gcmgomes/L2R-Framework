#include "ranker.h"

#include <iostream> 

namespace bayes {
namespace branch_and_bound {
namespace inference {

Ranker::Ranker(const Graph& network, const ::util::Discretizer& discretizer)
    : network_(network), discretizer_(discretizer) {
}

long double Ranker::Rank(Instance instance) const {
  long double score = 0;
  for (auto value : network_.variables().at(0).categories()) {
    instance.mutable_values()[0] = value;
    long double probability =
        network_.variables().at(0).cp_table()->Query(instance);
    score += probability * value;
  }
  return score;
}

long double Ranker::Rank(const ::base::Document& document) const {
  Instance instance(document, discretizer_);
  return Rank(instance);
}

}  // namespace inference
}  // namespace branch_and_bound
}  // namespace bayes
