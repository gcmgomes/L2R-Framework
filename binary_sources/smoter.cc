#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <ctime>
#include "../base/dataset.h"
#include "../knn/k_nearest_neighbours.h"
#include "../knn/smote.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc < 7) {
    cerr << argv[0] << " [input dataset] [output dataset] [relevance label] "
                       "[extra relevants percentage] [knn size] [use feedback]"
         << endl;
    return 0;
  }
  unsigned global = time(NULL);//1434568994;//time(NULL);
  std::cerr << "Seed " << global << std::endl;
  srand(global);
  unsigned relevance_label = 0, use_feedback = 0, knn_size = 0;
  double extras = 0;
  sscanf(argv[3], "%u", &relevance_label);
  sscanf(argv[4], "%lf", &extras);
  sscanf(argv[5], "%u", &knn_size);
  sscanf(argv[6], "%u", &use_feedback);
  base::Dataset dataset;

  dataset.Parse(argv[1]);

  auto query = dataset.begin();
  while (query != dataset.end()) {
    ::knn::Smote smote(*query, relevance_label, knn_size);

    smote.Populate(extras, use_feedback > 0);

    smote.AugmentQuery(*query);
    ++query;
  }

  dataset.Write(argv[2]);
}
