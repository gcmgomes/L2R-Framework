#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <ctime>
#include "../base/dataset.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc < 3) {
    cerr << argv[0] << " [input dataset] [output dataset]" << endl;
    return 0;
  }
  base::Dataset dataset;
  dataset.Parse(argv[1]);
  dataset.Write(argv[2]);
}
