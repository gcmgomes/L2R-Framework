// A utility class meant to ease the discretization process necessary in many
// differente L2R techniques. Right now, only 2 methods are supported: Uniform
// Bin Length and Tree Based Unsupervised (TUBE). To use an instances of this
// class, build it with the appropriate |mode| value and then call the
// Initialize method. Afterwards, the splitting points will be stored in
// |frontiers_| and available for use through the accessor method.
// IMPORANT: this class (for now) only handles values in the range [0, 1]. If
// necessary we will contemplate extending it to arbitrary ranges.
#ifndef _RI_2015_1_UTIL_DISCRETIZER_H_
#define _RI_2015_1_UTIL_DISCRETIZER_H_

#include <cstdlib>
#include <iostream>
#include <vector>
#include "../base/query.h"

namespace util {

class Discretizer {
 public:
  enum class Mode {
    UNIFORM_BIN_LENGTH,       // [0, 1] / |bin_count|.
    TREE_BASED_UNSUPERVISED,  // Discretizes the features using TUBE
                              // [Schmidberger and Frank, PKDD 2005].
  };

  const std::unordered_map<unsigned, std::vector<double> >& frontiers() const {
    return frontiers_;
  }

  Discretizer(Mode mode, unsigned bin_count)
      : bin_count_(bin_count), mode_(mode){};

  unsigned bin_count() const {
    return bin_count_;
  }

  Mode mode() const {
    return mode_;
  }

  // Initializes the |bins_| vector with the valid intervals. Depends directly
  // on |mode_|.
  void Initialize(const ::base::Query& query);

  // Initializes the discretizer based on |values| for |feature_id|.
  void Initialize(unsigned feature_id, const std::vector<double>& values);

  // Discretizes |feature_value| according to |mode_| and |feature_id|.
  unsigned Discretize(unsigned feature_id, double feature_value) const;

  // Returns the upper limit of the bin represented by |discretized_value| for
  // feature |feature_id|.
  double UpperBinLimit(unsigned feature_id, unsigned discretized_value) const;

 private:
  // Constructs the tree for the given |values| vector and stores in the
  // appropriate place.
  void InitializeTube(unsigned feature_id, std::vector<double> values);

  // Perform discretization of |feature_value| for uniform bin lengths.
  unsigned DiscretizeUniformBinLength(double feature_value) const;

  // Perform discretization of |feature_value| from feature |feature_id| with
  // TUBE discretizers.
  unsigned DiscretizeTube(unsigned feature_id, double feature_value) const;

  // The amount of bins we want to divide our range in.
  unsigned bin_count_;

  // The mode we are using to
  Mode mode_;

  // The mapping |feature_id| -> |range split points|. This stucture only stores
  // the right ends of the intervals! As such a = frontiers[0] represents the
  // interval [0, a), and for general i, a_i = frontiers[i] represents
  // [a_{i-1}, a_i).
  std::unordered_map<unsigned, std::vector<double> > frontiers_;
};

}  // namespace util

#endif
