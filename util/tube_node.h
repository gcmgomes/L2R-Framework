// The inner nodes that compose a TUBE tree. They have information pertaining to
// the limits of the range each of them represents, including the best
// log-likelihood gain obtained. There is no real reason to use this class
// outside of TubeTree.
#ifndef _RI_2015_1_UTIL_TUBE_NODE_H_
#define _RI_2015_1_UTIL_TUBE_NODE_H_

#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>
#include "tube_input.h"

namespace util {

class TubeNode {
 public:
  TubeNode(const TubeInput* tube_input, double lower, double upper)
      : discretized_value_(0), lower_(lower), mid_(lower - 1000.0),
        upper_(upper), log_likelihood_(-1000000.0) {
    tube_input_ = tube_input;
    EPS = tube_input_->minimum_gap();
    MIN_RANGE_SUPPORT =
        EPS * (tube_input_->values().back() - tube_input_->values().front());
  };

  unsigned discretized_value() const {
    return discretized_value_;
  }

  unsigned& mutable_discretized_value() {
    return discretized_value_;
  }

  double lower() const {
    return lower_;
  }

  double& mutable_lower() {
    return lower_;
  }

  double mid() const {
    return mid_;
  }

  double& mutable_mid() {
    return mid_;
  }

  double upper() const {
    return upper_;
  }

  double& mutable_upper() {
    return upper_;
  }

  TubeNode* left() const {
    return left_.get();
  }

  TubeNode* right() const {
    return right_.get();
  }

  double log_likelihood() const {
    return log_likelihood_;
  }

  static bool verbose() {
    return verbose_;
  }

  static std::string StringHeader();

  std::string ToString() const;

  // Fills |mid_| and |log_likelihood| according to the optimal splitting
  // log-likelihood. Returns the same pair as DividedLikelihood.
  std::pair<double, double> FindSplitPoint();

  // Uses the value of |mid_| to perform the basic bin split operation. Must
  // only be called after |FindSplitPoint| is used. Returns true if the split is
  // successful, false otherwise.
  bool Split();

 private:
  // Computes the maximum log-likelihood of a given split. Returns the pair
  // (split_point, likelihood).
  std::pair<double, double> DividedLikelihood(unsigned begin, unsigned split,
                                              unsigned end) const;
  // The discretized value/bin that this node represents.
  unsigned discretized_value_;

  // The lowest value inside the range. (closed interval)
  double lower_;

  // The point where we are going to divide the interval.
  double mid_;

  // The lowest value just outside the range. (open interval)
  double upper_;

  // The score associated with this node.
  double log_likelihood_;

  // The left subinterval.
  std::unique_ptr<TubeNode> left_;

  // The right subinterval.
  std::unique_ptr<TubeNode> right_;

  // The slacking constant associated with the splitting process.
  double EPS;

  // The minimum support of the bin.
  double MIN_RANGE_SUPPORT;

  // The input data to be used for our computations.
  const TubeInput* tube_input_;

  static const bool verbose_ = false;
};

}  // namespace util

#endif
