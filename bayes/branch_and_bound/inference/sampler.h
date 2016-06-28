#ifndef _RI_2015_1_BAYES_BRANCH_AND_BOUND_INFERENCE_SAMPLER_H_
#define _RI_2015_1_BAYES_BRANCH_AND_BOUND_INFERENCE_SAMPLER_H_

#include "../graph.h"
#include <cstdlib>
#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>

namespace bayes {
namespace branch_and_bound {
namespace inference {

class Sampler {
 public:
   Sampler(const Graph& network, unsigned seed = (unsigned) -1);

   Instance Sample();

 private:
   void Initialize();

   Graph network_;
   std::vector<unsigned> topological_order_;
   std::default_random_engine generator_;
};

}  // namespce inference
}  // namespce branch_and_bound
}  // namespace bayes

#endif

