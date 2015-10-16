#include <cstdlib>
#include <iostream>
#include <queue>
#include <unordered_map>
#include "../segmented_smote.h"
#include "../../../base/dataset.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc < 5) {
    cerr << argv[0]
         << " [input dataset] [number of bins] [knn's k] [output dataset]"
         << endl;
    return 0;
  }
  unsigned bin_count = 10, k = 5, relevance = 1;
  sscanf(argv[2], "%u", &bin_count);
  sscanf(argv[3], "%u", &k);
  base::Dataset dataset;
  dataset.Parse(argv[1]);
  auto query = dataset.begin();
  while (query != dataset.end()) {
    knn::segmented_smote::SegmentedSmote smote(*query, relevance, k, bin_count);
    smote.Populate(100, false);
    smote.AugmentQuery(*query);
    ++query;
  }
  dataset.Write(argv[4]);
}
