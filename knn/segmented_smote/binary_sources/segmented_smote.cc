#include <cstdlib>
#include <iostream>
#include <queue>
#include <random>
#include <unordered_map>
#include "../segmented_smote.h"
#include "../../../base/dataset.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc < 7) {
    cerr << argv[0]
         << " [input dataset] [discretizer (1 = TUBE)] [number of bins] [knn's k] [extra count] [random seed] [output dataset]"
         << endl;
    return 0;
  }
  unsigned mode = 0, bin_count = 10, k = 5, relevance = 1, seed = 0;
  double extras = 100.0;
  base::Dataset dataset;
  dataset.Parse(argv[1]);
  sscanf(argv[2], "%u", &mode);
  sscanf(argv[3], "%u", &bin_count);
  sscanf(argv[4], "%u", &k);
  sscanf(argv[5], "%lf", &extras);
  sscanf(argv[6], "%u", &seed);
  srand(seed);
  auto query = dataset.begin();
  ::util::Discretizer::Mode disc_mode = ::util::Discretizer::Mode::TREE_BASED_UNSUPERVISED;
  if(!mode) {
    disc_mode = ::util::Discretizer::Mode::UNIFORM_BIN_LENGTH;
  }
  int i = 0;
  while (query != dataset.end()) {
    knn::segmented_smote::SegmentedSmote smote(*query, relevance, k, disc_mode, bin_count);
    std::cerr << "Running on query " << ++i << "/" << dataset.end() - dataset.begin() << endl;
    smote.Populate(extras, false);
    smote.AugmentQuery(*query);
    ++query;
  }
  dataset.Write(argv[7]);
}
