#ifndef _L2RF_BAYES_BRANCH_AND_BOUND_REFERENCE_EXTERNAL_QUEUE_H_
#define _L2RF_BAYES_BRANCH_AND_BOUND_REFERENCE_EXTERNAL_QUEUE_H_

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include <unordered_map>
#include <queue>
#include "external_graph_reference.h"
#include "../../../util/data_structures/min_max_heap.h"

namespace bayes {
namespace branch_and_bound {

class ReferenceExternalQueue {
 public:
  ReferenceExternalQueue(const std::string& file_path);

  bool empty();

  Graph min();

  Graph max();

  void push(const Graph& key);

  void pop_min();
  void pop_max();

  size_t size() const {
    return reference_heap_.size();
  }

  const std::string& file_path() const {
    return file_path_;
  }

  void Clear();

 private:
  void Initialize(const std::string& file_path);

  void OpenRepository(const std::string& file_path);

  Graph FetchGraph(const ExternalGraphReference& reference);

  std::string file_path_;
  std::streampos tellp_;
  ::util::data_structures::MinMaxHeap<ExternalGraphReference> reference_heap_;
  std::fstream repository_;
};

}  // namespce branch_and_bound
}  // namespace bayes

#endif
