#include <cassert>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <cmath>
#include <unordered_set>
#include "../variable.h"
#include "../../../base/dataset.h"
#include "../../../util/discretizer.h"

using namespace std;

int main(int argc, char** argv) {
  string file_path = "/var/tmp/data.txt";
  if (argc > 1) {
    file_path = argv[1];
  }
  util::Discretizer disc(util::Discretizer::Mode::TREE_BASED_UNSUPERVISED, 2);
  vector<bayes::branch_and_bound::Instance> instances;
  bayes::branch_and_bound::Instance::ParseDataset(file_path, disc, instances);
  vector<bayes::branch_and_bound::Cache> caches;
  bayes::branch_and_bound::Cache::InitializeCaches("/var/tmp/", instances,
                                                   caches, 100000);
  vector<bayes::branch_and_bound::ExternalQueue> external_queues;
  bayes::branch_and_bound::ExternalQueue::InitializeExternalQueues(
      "/var/tmp/", instances.front().values().size(), 1000000, external_queues);
  vector<bayes::branch_and_bound::Variable> variables;
  bayes::branch_and_bound::Variable::InitializeVariables(
      instances, variables, external_queues, caches);
  auto it = variables.begin();
  ++it;
  while (it != variables.end()) {
    it->BuildCache(instances, variables);
    bayes::branch_and_bound::ExternalQueue::ClearExternalQueue(
        "/var/tmp/", it->variable_id());
    ++it;
  }
  return 0;
}
