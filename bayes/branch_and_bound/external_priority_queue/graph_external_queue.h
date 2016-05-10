#ifndef _L2RF_BAYES_BRANCH_AND_BOUND_GRAPH_EXTERNAL_QUEUE_H_
#define _L2RF_BAYES_BRANCH_AND_BOUND_GRAPH_EXTERNAL_QUEUE_H_

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include <unordered_map>
#include <queue>
#include "../graph.h"

namespace bayes {
namespace branch_and_bound {

class GraphExternalQueue {
 public:
  GraphExternalQueue(const std::string& file_path, size_t queue_limit);

  bool empty();

  const Graph& front() const;

  void push(const Graph& key);

  void pop();

  size_t size() const {
    return size_;
  }

  size_t& mutable_queue_limit() {
    return queue_limit_;
  }

  const std::string& file_path() const {
    return file_path_;
  }

  void Clear();

 private:
  void Initialize(const std::string& file_path);

  void OpenRepository(const std::string& file_path);

  void FetchFromRepository();

  size_t queue_limit_;
  size_t size_;
  std::string file_path_;
  std::streampos tellg_;
  std::streampos tellp_;
  std::queue<Graph> queue_;
  std::fstream repository_;
};

}  // namespce branch_and_bound
}  // namespace bayes

#endif
