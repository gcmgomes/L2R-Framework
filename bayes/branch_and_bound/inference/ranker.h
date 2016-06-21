#ifndef _RI_2015_1_BAYES_BRANCH_AND_BOUND_INFERENCE_RANKER_H_
#define _RI_2015_1_BAYES_BRANCH_AND_BOUND_INFERENCE_RANKER_H_

#include "../graph.h"
#include "base/dataset.h"
#include <cstdlib>
#include <iostream>
#include <unordered_set>

namespace bayes {
namespace branch_and_bound {
namespace inference {

class Ranker {
 public:
   Ranker(const Graph& network, const ::util::Discretizer& discretizer);

   long double Rank(Instance instance) const;

   long double Rank(const ::base::Document& document) const;

 private:
   Graph network_;
   ::util::Discretizer discretizer_;
};

}  // namespce inference
}  // namespce branch_and_bound
}  // namespace bayes

#endif

