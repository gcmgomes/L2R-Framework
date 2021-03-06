#include <cstdlib>
#include <iostream>
#include <queue>
#include <unordered_map>
#include "smote_segmenter.h"
#include "../../util/discretizer.h"
#include "../../base/dataset.h"

using namespace std;

int main(int argc, char** argv) {
  if(argc < 2) {
    cerr << argv[0] << " [dataset]" << endl;
    return 0;  
  }
  unsigned bin_count = 10;
  base::Dataset dataset;
  dataset.Parse(argv[1]);
  base::Query query = *(dataset.begin());
  util::Discretizer disc(util::Discretizer::Mode::TREE_BASED_UNSUPERVISED, bin_count);
  disc.Initialize(query);
  knn::segmented_smote::SegmentationMetadata meta(&disc, query);
  auto i = query.cbegin(), j = i;
  j++;
  knn::segmented_smote::SmoteSegmenter segmenter(&disc, &meta, *i, *j);
  base::Document result(0);
  cout << "Generating synthetic between" << endl;
  cout << i->ToString() << endl << " and " << endl;
  cout << j->ToString() << endl;
  unsigned score = segmenter.Segment(result);
  cout << result.ToString() << " | Scored = " << score << endl;
}

