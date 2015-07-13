#ifndef _RI_2015_1_UTIL_NORMALIZER_H_
#define _RI_2015_1_UTIL_NORMALIZER_H_

#include <cstdlib>
#include <iostream>
#include <vector>
#include "../base/query.h"

namespace util {

class Normalizer {
 public:
  enum class Mode {
    STANDARD_SCORE,  // (X - mean(X)) / standard_deviation(X).
    FEATURE_SCALING  // (X - max(X)) / (max(X) - min(X)).
  };

  Normalizer(){};

  // Normalizes |query| according to |normalization_mode|
  void Normalize(const Mode mode, ::base::Query& query);

 private:
  void NormalizeStandardScore(::base::Query& query);

  void NormalizeFeatureScaling(::base::Query& query);
};

}  // namespace base

#endif
