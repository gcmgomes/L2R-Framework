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
#include "../indexed_variable.h"

using namespace std;

int main(int argc, char** argv) {
  string file_path = "/var/tmp/data.txt";
  if (argc > 1) {
    file_path = argv[1];
  }
  util::Discretizer disc(util::Discretizer::Mode::TREE_BASED_UNSUPERVISED, 10);
  vector<bayes::branch_and_bound::Instance> instances;
  bayes::branch_and_bound::Instance::ParseDataset(file_path, disc, instances);
  vector<bayes::branch_and_bound::Cache> caches;
  bayes::branch_and_bound::Cache::InitializeCaches("/var/tmp/", instances,
                                                   caches, 100000);
  vector<bayes::branch_and_bound::ExternalQueue> external_queues;
  bayes::branch_and_bound::ExternalQueue::InitializeExternalQueues(
      "/var/tmp/", instances, 1000000, external_queues);
  bayes::branch_and_bound::InvertedIndex index(instances);
  instances.clear();
  vector<bayes::branch_and_bound::Variable> variables;
  bayes::branch_and_bound::Variable::InitializeVariables(
      index, variables, external_queues, caches);
  auto it = variables.begin();
  ++it;
  while (it != variables.end()) {
    it->BuildCache(index, variables);
    ++it;
  }
  bayes::branch_and_bound::ExternalQueue::ClearExternalQueues(
      "/var/tmp/", external_queues);
  return 0;
}
