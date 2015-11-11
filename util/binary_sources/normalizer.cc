#include <iostream>
#include <fstream>
#include "../normalizer.h"
#include "../../base/dataset.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc < 3) {
    cerr << argv[0] << " [input dataset] [output dataset]" << endl;
    return 0;
  }
  base::Dataset d;

  d.Parse(argv[1]);
  util::Normalizer norm;
  auto query = d.mutable_queries().begin();
  while (query != d.mutable_queries().end()) {
    norm.Normalize(util::Normalizer::Mode::FEATURE_SCALING, *query);
    ++query;
  }
  d.Write(argv[2]);
}
