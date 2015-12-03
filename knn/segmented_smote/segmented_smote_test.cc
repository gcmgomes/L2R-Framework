#include <cstdlib>
#include <iostream>
#include <queue>
#include <unordered_map>
#include "segmented_smote.h"
#include "../../base/dataset.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc < 2) {
    cerr << argv[0] << " [dataset]" << endl;
    return 0;
  }
  unsigned bin_count = 10, k = 5, relevance = 1;
  base::Dataset dataset;
  dataset.Parse(argv[1]);
  base::Query query = *(dataset.begin());
  knn::segmented_smote::SegmentedSmote smote(
      query, relevance, k, ::util::Discretizer::Mode::TREE_BASED_UNSUPERVISED,
      bin_count);
  smote.Populate(100, false);
  smote.AugmentQuery(query);
  auto i = query.cbegin();
  while (i != query.cend()) {
    if (i->relevance_label() == relevance) {
      cout << i->ToString() << endl;
    }
    ++i;
  }
}
