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
#include "../external_queue.h"

using namespace std;

int main() {
  size_t size = 16;
  bayes::branch_and_bound::ExternalQueue ext_queue(
      "/Users/gcmgomes/git_repositories/learning_to_rank_parse/bayes/"
      "branch_and_bound/tmp/test_queue.txt",
      1);

  int i = 0;
  {
    bayes::branch_and_bound::Bitset bitset(size);
    while (i < size) {
      bitset.Set(i, true);
      cerr << "Pushing bitset: " << bitset.ToString() << endl;
      ext_queue.push(bitset);
      i += 2;
    }
    cerr << endl;
    while (!ext_queue.empty()) {
      cerr << "Popping bitset: " << ext_queue.front().ToString() << endl;
      ext_queue.pop();
    }
  }
  bayes::branch_and_bound::Bitset bitset(size);
  i = 0;
  cerr << endl;
  while (i < size) {
    bitset.Set(i, true);
    cerr << "Pushing bitset: " << bitset.ToString() << endl;
    ext_queue.push(bitset);
    i += 2;
  }
  cerr << endl;
  while (!ext_queue.empty()) {
    cerr << "Popping bitset: " << ext_queue.front().ToString() << endl;
    ext_queue.pop();
  }
  cerr << "Done" << endl;
  return 0;
}
