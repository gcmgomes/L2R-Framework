#include <cstdlib>
#include <iostream>
#include <queue>
#include <unordered_map>
#include "segmentation_metadata.h"
#include "../../util/discretizer.h"
#include "../../base/dataset.h"

using namespace std;

void print(const std::unordered_map<
    unsigned, std::unordered_map<unsigned, unsigned>>& m) {
  auto i = m.cbegin();
  while (i != m.cend()) {
    auto j = i->second.cbegin();
    while (j != i->second.cend()) {
      std::cout << "Feature " << i->first << ", value " << j->first
                << " = " <<  j->second << std::endl;
      ++j;
    }
    ++i;
  }
}

int main(int argc, char** argv) {
  if (argc < 2) {
    cerr << argv[0] << " [dataset]" << endl;
    return 0;
  }
  unsigned bin_count = 2;
  base::Dataset dataset;
  dataset.Parse(argv[1]);
  base::Query query = *(dataset.begin());
  util::Discretizer disc(util::Discretizer::Mode::TREE_BASED_UNSUPERVISED,
                         bin_count);
  disc.Initialize(query);
  knn::segmented_smote::SegmentationMetadata meta(&disc, query);
  print(meta.feature_value_map());
  auto i = query.cbegin();
  while (i != query.cend()) {
    cout << i->ToString() << " | Scored = " << meta.Score(*i) << endl;
    ++i;
  }
}
