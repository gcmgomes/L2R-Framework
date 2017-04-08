#include "tube_input.h"

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
  int i = 0;
  while (i < input.values().size()) {
    cout << "i: " << input.values().at(i) << " " << input.counts().at(i) << " "
         << input.sum_prefix().at(i) << endl;
    i++;
  }
}
