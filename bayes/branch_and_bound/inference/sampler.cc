#include "sampler.h"
#include <chrono>
#include <queue>

namespace bayes {
namespace branch_and_bound {
namespace inference {

Sampler::Sampler(const Graph& network, unsigned seed) {
  network_ = network;
  if (seed == (unsigned)-1) {
    seed = std::chrono::system_clock::now().time_since_epoch().count();
  }
  generator_ = std::default_random_engine(seed);
  Initialize();
}

Instance Sampler::Sample() {
  Instance instance;
  instance.mutable_values().resize(network_.variables().size());
  // Check the variables as specified in the variable ordering.
  for (const auto& variable : topological_order_) {
    std::vector<double> probabilities;
    // Get the conditional on the already determined variables.
    for (const auto& category :
         network_.variables().at(variable).categories()) {
      instance.mutable_values()[variable] = category;
      probabilities.push_back(
          network_.variables().at(variable).cp_table()->Query(instance));
    }
    std::discrete_distribution<unsigned> distribution(probabilities.begin(),
                                                      probabilities.end());
    unsigned category_id = distribution(generator_);
    instance.mutable_values()[variable] =
        network_.variables().at(variable).categories().at(category_id);
  }
  return instance;
}

void Sampler::Initialize() {
  std::vector<std::vector<unsigned>> graph(network_.variables().size());
  std::queue<unsigned> queue;
  std::unordered_map<unsigned, unsigned> degrees;
  for (const auto& variable : network_.variables()) {
    std::vector<unsigned> high_bits = variable.parent_set().high_bits();
    degrees[variable.variable_id()] = high_bits.size();
    if (high_bits.empty()) {
      queue.push(variable.variable_id());
      topological_order_.push_back(variable.variable_id());
    }
    for (const auto& parent : high_bits) {
      graph[parent].push_back(variable.variable_id());
    }
  }

  while (!queue.empty()) {
    unsigned u = queue.front();
    queue.pop();
    for (const auto& v : graph[u]) {
      degrees[v]--;
      if (!degrees[v]) {
        topological_order_.push_back(v);
        queue.push(v);
      }
    }
  }
}

}  // namespce inference
}  // namespce branch_and_bound
}  // namespace bayes
