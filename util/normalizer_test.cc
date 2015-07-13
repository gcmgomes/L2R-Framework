#include <iostream>
#include <fstream>
#include "../base/dataset.h"
#include "normalizer.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc < 2) {
    cerr << argv[0] << " [dataset]" << endl;
    return 0;
  }
  base::Dataset d;

  d.Parse(argv[1]);
  util::Normalizer norm;
  auto query = d.mutable_queries().begin();
  while (query != d.mutable_queries().end()) {
    norm.Normalize(util::Normalizer::Mode::FEATURE_SCALING, *query);
    auto document = query->documents().begin();
    while (document != query->documents().end()) {
      cout << document->ToString() << endl;
      ++document;
    }
    ++query;
  }
}
