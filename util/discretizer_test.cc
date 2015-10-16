#include <iostream>
#include <fstream>
#include "discretizer.h"

using namespace std;

int main() {
  int n = 0, bin_count = 0;
  cin >> n >> bin_count;
  util::Discretizer discretizer(bin_count);
  while (n) {
    double value = 0;
    cin >> value;
    cout << value << " = "
         << discretizer.Discretize(util::Discretizer::Mode::UNIFORM_BIN_LENGTH,
                                   value) << endl;
    n--;
  }
}
