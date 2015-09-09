#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <ctime>
#include "../base/dataset.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc < 3) {
    cerr << argv[0] << " [input dataset] [fold count] [output directory]"
         << endl;
    return 0;
  }
  unsigned fold_count = 0, queries_per_fold = 0;
  sscanf(argv[2], "%u", &fold_count);
  base::Dataset dataset;
  cout << "Parsing" << endl;
  dataset.Parse(argv[1]);
  cout << "Done Parsing" << endl;
  queries_per_fold =
      ceil((double)dataset.queries().size() / (double)fold_count);

  string out_dir = argv[3];
  unsigned i = 0, fold_id = 1;
  while (i < dataset.queries().size()) {
    char num[256];
    sprintf(num, "fold_%u.txt", fold_id);
    string file = out_dir;
    file += num;
    cout << file << endl;

    base::Dataset d;
    unsigned count = 0;
    while (i < dataset.queries().size() && count < queries_per_fold) {
      d.mutable_queries().push_back(dataset.mutable_queries()[i]);
      i++;
      count++;
    }
    if (d.queries().size() > 0) {
      d.Write(file);
    }
    fold_id++;
  }
}
