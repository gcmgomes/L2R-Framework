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
#include "../inverted_index.h"

using namespace std;

int main(int argc, char** argv) {
  string file_path = "/var/tmp/data.txt";
  if (argc > 1) {
    file_path = argv[1];
  }
  util::Discretizer disc(util::Discretizer::Mode::TREE_BASED_UNSUPERVISED, 2);
  vector<bayes::branch_and_bound::Instance> instances;
  bayes::branch_and_bound::Instance::ParseDataset(file_path, disc, instances);
  bayes::branch_and_bound::InvertedIndex index(instances);
  cout << index.ToString() << endl;
  auto s_1_1 = index.at(1).entries().at(1);
  auto s_2_1 = index.at(2).entries().at(1);
  auto inter = s_1_1;
  bayes::branch_and_bound::InvertedIndex::MinimalIntersection(s_1_1, s_2_1, inter);
  auto it = inter.cbegin();
  while(it != inter.cend()) {
    cout << *it << " ";
    ++it;
  }
  cout << endl;
  return 0;
}
