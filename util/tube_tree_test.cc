#include <iostream>
#include <fstream>
#include "tube_tree.h"

using namespace std;

int main() {
  int n = 0, bin_count = 0;
  cin >> n >> bin_count;
  vector<double> values(n);
  while (n) {
    cin >> values[n-1];
    n--;
  }
  sort(values.begin(), values.end());
  util::TubeTree tree(bin_count);
  tree.Build(values);
}
