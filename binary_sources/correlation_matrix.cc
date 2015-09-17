#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <queue>
#include "../base/dataset.h"
#include "../util/statistics.h"

using namespace std;

void FeatureList(const ::base::Query& query, set<unsigned>& features) {
  auto doc = query.cbegin();
  while (doc != query.cend()) {
    auto feature = doc->cbegin();
    while (feature != doc->cend()) {
      features.insert(feature->first);
      ++feature;
    }
    ++doc;
  }
}

int main(int argc, char** argv) {
  if (argc < 3) {
    cerr << argv[0] << " [input file] [statistic]" << endl;
    cerr << " [statistic] : 0 -> Pearson's Rho" << endl;
    cerr << "               1 -> Kendall's Tau" << endl;
    return 0;
  }

  ::base::Dataset* dataset = new ::base::Dataset();
  dataset->Parse(argv[1]);

  ::base::Query collapsed(1, dataset->cbegin()->dimension_count());
  dataset->Collapse(collapsed);
  delete dataset;

  ::util::Statistics statistics(collapsed);

  set<unsigned> features;
  FeatureList(collapsed, features);

  unsigned coefficient;
  sscanf(argv[2], "%u", &coefficient);
  auto f1 = features.begin();
  while (f1 != features.end()) {
    cout << statistics.means().at(*f1) << " "
         << statistics.standard_deviations().at(*f1) << endl;
    auto f2 = f1;
    bool extra_space = false;
    ++f2;
    while (f2 != features.end()) {
      cerr << "Computing for (" << *f1 << ", " << *f2 << ")" << endl;
      if (extra_space) {
        cout << " ";
      }
      extra_space = true;
      if (coefficient) {
        cout << statistics.FastKendallTau(collapsed, *f1, *f2);
      } else {
        cout << statistics.PearsonRho(collapsed, *f1, *f2);
      }
      ++f2;
    }
    if (extra_space) {
      cout << endl;
    }
    ++f1;
  }
}
