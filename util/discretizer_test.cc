#include <iostream>
#include <fstream>
#include "discretizer.h"

using namespace std;

int main() {
  int n = 0, bin_count = 0;
  cin >> n >> bin_count;
  vector<double> values(n);
  util::Discretizer discretizer(util::Discretizer::Mode::TREE_BASED_UNSUPERVISED, bin_count);
  while (n) {
    double value = 0;
    cin >> values[n - 1];
    n--;
  }
  discretizer.Initialize(0, values);
  auto i = discretizer.frontiers().cbegin();
  while (i != discretizer.frontiers().cend()) {
    auto j = i->second.cbegin();
    cout << "for feauture " << i->first << " we have the split points:\n0";
    while (j != i->second.cend()) {
      cout << " " << *j;
      ++j;
    }
    cout << endl;
    ++i;
  }
}
