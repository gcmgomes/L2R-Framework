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
#include "../bitset.h"

using namespace std;

int main() {
  size_t size = 16;
  bayes::branch_and_bound::Bitset bitset(size);
  unordered_set<bayes::branch_and_bound::Bitset> bitsets;
  int i = 0;
  while (i < size) {
    bitset.Set(i, true);
    bitsets.insert(bitset);
    i += 2;
  }
  cout << bitset.ToString() << endl;
  bitset.Set(size - 1, false);
  cout << bitset.ToString() << endl;
  cout << "Hashed bitsets:" << endl;
  auto b = bitsets.begin();
  while (b != bitsets.end()) {
    cout << b->ToString() << endl;
    ++b;
  }
  return 0;
}
