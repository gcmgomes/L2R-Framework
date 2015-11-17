#ifndef _RI_2015_1_UTIL_TUBE_INPUT_H_
#define _RI_2015_1_UTIL_TUBE_INPUT_H_

#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

namespace util {

class TubeInput {
 public:
  // Before instantiation, |vals| must be sorted.
  TubeInput(const std::vector<double>& vals);

  const std::vector<double>& values() const {
    return values_;
  }

  const std::vector<double>& counts() const {
    return counts_;
  }

  const std::vector<double>& sum_prefix() const {
    return sum_prefix_;
  }

  double minimum_gap() const {
    return minimum_gap_;
  }

  // Returns the amount of points in the range [lower, upper).
  unsigned CountPoints(double lower, double upper) const;

  // Returns the amount of points in the range [values_[begin], values[end]).
  unsigned CountPoints(unsigned begin, unsigned end) const;

 private:
  void Initialize(const std::vector<double>& vals);

  std::vector<double> values_;

  std::vector<double> counts_;

  std::vector<double> sum_prefix_;

  double minimum_gap_;
};

}  // namespace util

#endif
