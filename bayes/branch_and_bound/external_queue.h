#ifndef _L2RF_BAYES_BRANCH_AND_BOUND_EXTERNAL_QUEUE_H_
#define _L2RF_BAYES_BRANCH_AND_BOUND_EXTERNAL_QUEUE_H_

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include <unordered_map>
#include <queue>
#include "bitset.h"
#include "instance.h"

namespace bayes {
namespace branch_and_bound {

class ExternalQueue {
 public:
  ExternalQueue(size_t queue_limit);

  bool empty();

  const Bitset& front() const;

  void push(const Bitset& bitset);

  void pop();

  size_t size() const {
    return size_;
  }

  static void InitializeExternalQueues(
      const std::string& directory_root_path,
      const std::vector<Instance>& instances, size_t queue_limit,
      std::vector<ExternalQueue>& external_queues);

  void OpenRepository(const std::string& file_path);

 private:
  void FetchFromRepository();

  size_t queue_limit_;
  size_t size_;
  std::streampos tellg_;
  std::streampos tellp_;
  std::queue<Bitset> queue_;
  std::fstream repository_;
};

}  // namespce branch_and_bound
}  // namespace bayes

#endif
