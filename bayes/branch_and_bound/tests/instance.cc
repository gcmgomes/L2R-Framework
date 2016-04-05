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
#include "../instance.h"
#include "../../../base/dataset.h"
#include "../../../util/discretizer.h"

using namespace std;

int main() {
  string file_path =
      "/Users/gcmgomes/git_repositories/learning_to_rank_parse/bayes/x.txt";
  util::Discretizer disc(util::Discretizer::Mode::TREE_BASED_UNSUPERVISED, 2);
  vector<bayes::branch_and_bound::Instance> instances;
  bayes::branch_and_bound::Instance::ParseDataset(file_path, disc, instances);
  auto it = instances.cbegin();
  while (it != instances.cend()) {
    cout << it->ToString() << endl;
    ++it;
  }
  return 0;
}
