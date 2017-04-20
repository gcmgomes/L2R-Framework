#ifndef _RI_2015_1_UTIL_STATISTICS_H_
#define _RI_2015_1_UTIL_STATISTICS_H_

#include <cstdlib>
#include <iostream>
#include <vector>
#include "../base/query.h"

namespace util {

class Statistics {
 public:
  Statistics(const ::base::Query& query);

  const std::unordered_map<unsigned, double>& means() const {
    return means_;
  }

  const std::unordered_map<unsigned, double>& standard_deviations() const {
    return standard_deviations_;
  }
  
  // Calculates Pearson's Rho correlation coefficient between features
  // |feature_1|
  // and |feature_2|.
  double PearsonRho(const ::base::Query& query, unsigned feature_1,
                    unsigned feature_2);

  // Calculates Kendall's Tau correlation coefficient between features
  // |feature_1|
  // and |feature_2|.
  double KendallTau(const ::base::Query& query, unsigned feature_1,
                    unsigned feature_2);

  // Same as KendallTau, but takes O(n log n).
  double FastKendallTau(const ::base::Query& query, unsigned feature_1,
                        unsigned feature_2);

 private:
  void init(const ::base::Query& query);

  std::unordered_map<unsigned, double> means_;

  std::unordered_map<unsigned, double> standard_deviations_;
};

}  // namespace util

#endif
