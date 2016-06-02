#ifndef _L2RF_BAYES_BRANCH_AND_BOUND_EXTERNAL_GRAPH_REFERENCE_H_
#define _L2RF_BAYES_BRANCH_AND_BOUND_EXTERNAL_GRAPH_REFERENCE_H_

#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>
#include "../graph.h"

namespace bayes {
namespace branch_and_bound {

class ExternalGraphReference {
 public:
  ExternalGraphReference(const Graph& graph, std::streampos stream_pos)
      : score_(graph.score()), stream_pos_(stream_pos) {
  }

  long double score() const {
    return score_;
  }

  std::streampos stream_pos() const {
    return stream_pos_;
  }

  bool operator< (const ExternalGraphReference& r) const {
    return this->score_ < r.score();
  }

 private:
  long double score_;
  std::streampos stream_pos_;
};

}  // namespce branch_and_bound
}  // namespace bayes

#endif
