#include "tube_node.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
  unsigned n = 0;
  cin >> n;
  vector<double> values(n);
  while (n) {
    cin >> values[n - 1];
    n--;
  }
  sort(values.begin(), values.end());
  util::TubeInput input(values);
  util::TubeNode node(&input, values[0], values.back());
  pair<double, double> p = node.FindSplitPoint();
  cout << endl
       << endl
       << "Split point is " << p.first << " with L = " << p.second << endl
       << endl;
}
