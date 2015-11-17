#include <iostream>
#include <fstream>
#include "discretizer.h"
#include "../base/dataset.h"

using namespace std;

int main(int argc, char** argv) {
  if(argc < 3) {
    cerr << argv[0] << " [input dataset] [bin count]" << endl;
    return 0;
  }
  unsigned bin_count = 0;
  sscanf(argv[2], "%u", &bin_count);
  util::Discretizer discretizer(
      bin_count, util::Discretizer::Mode::TREE_BASED_UNSUPERVISED);
  base::Dataset dataset;
  dataset.Parse(argv[1]);

  base::Query query(0, 64);
  dataset.Collapse(query);
  discretizer.Initialize(query);
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

