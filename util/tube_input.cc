#include "tube_input.h"

namespace util {
TubeInput::TubeInput(const std::vector<double>& vals) {
  Initialize(vals);
}

unsigned TubeInput::CountPoints(double lower, double upper) const {
  unsigned begin = std::lower_bound(values_.cbegin(), values_.cend(), lower) -
                   values_.cbegin(),
           end = std::lower_bound(values_.cbegin(), values_.cend(), upper) -
                 values_.cbegin();
  if(begin == end) {
    return 0;
  }
  end--;
  std::cerr << "Looked up the indices of " << lower << " = " << begin << " and " << upper << " = " << end << std::endl;
  return CountPoints(begin, end);
}

unsigned TubeInput::CountPoints(unsigned begin, unsigned end) const {
  end = std::min(end, (unsigned)sum_prefix_.size());
  return sum_prefix_.at(end) - sum_prefix_.at(begin) + counts_.at(begin);
}

void TubeInput::Initialize(const std::vector<double>& vals) {
  int i = 0;
  double current = *(vals.begin()) - 1;
  while (i < vals.size()) {
    if (current == vals[i]) {
      counts_.back()++;
    } else {
      values_.push_back(vals[i]);
      current = vals[i];
      counts_.push_back(1);
    }
    i++;
  }
  i = 1;
  sum_prefix_.resize(counts_.size());
  sum_prefix_[0] = counts_[0];
  while (i < counts_.size()) {
    sum_prefix_[i] = sum_prefix_[i - 1] + counts_[i];
    i++;
  }
}
}  // namespace util
