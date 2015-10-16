#ifndef _RI_2015_1_UTIL_DISCRETIZER_H_
#define _RI_2015_1_UTIL_DISCRETIZER_H_

#include <cstdlib>
#include <iostream>
#include <vector>

namespace util {

class Discretizer {
 public:
  enum class Mode {
    UNIFORM_BIN_LENGTH,  // [0, 1] / |bin_count|.
  };

  unsigned bin_count() const {
    return bin_count_;
  }

  Discretizer(unsigned bin_count) : bin_count_(bin_count){};

  // Discretizes |feature_value| according to |mode|.
  double Discretize(const Mode mode, double feature_value) const;

 private:
  double DiscretizeUniformBinLength(double feature_value) const;

  unsigned bin_count_;
};

}  // namespace util

#endif
