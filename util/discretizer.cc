#include <cmath>
#include "discretizer.h"

namespace util {

double Discretizer::Discretize(const Mode mode, double feature_value) const {
  double value = -1;
  switch (mode) {
    case Mode::UNIFORM_BIN_LENGTH:
      value = DiscretizeUniformBinLength(feature_value);
      break;
  }
  return value;
}

double Discretizer::DiscretizeUniformBinLength(double feature_value) const {
  double b = bin_count_;
  if (feature_value >= 1) {
    return b - 1;
  }
  return floor(feature_value * b);
}

}  // namespace util

