#include "../base/dataset.h"
#include "../bayes/branch_and_bound/instance.h"
#include "../util/discretizer.h"
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
  ios::sync_with_stdio(false);

  if (argc != 3) {
    std::cerr << "Usage: ./" << argv[0]
              << " [input dataset filename] [output dataset filename]\n";
    return 1;
  }

  string input_file_path = argv[1];
  string output_file_path = argv[2];

  std::cerr << "Initializing Discretizer...\n";
  util::Discretizer disc(util::Discretizer::Mode::UNIFORM_BIN_LENGTH, 20);
  vector<bayes::branch_and_bound::Instance> instances;

  std::cerr << "Parsing Dataset...\n";
  bayes::branch_and_bound::Instance::ParseDataset(input_file_path, disc,
                                                  instances);

  freopen(output_file_path.c_str(), "w", stdout);
  for (bayes::branch_and_bound::Instance instance : instances) {
    std::cout << instance.ToPrintableString() << std::endl;
  }

  return 0;
}
