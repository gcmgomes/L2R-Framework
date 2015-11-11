#ifndef _RI_2015_1_UTIL_TUBE_TREE_H_
#define _RI_2015_1_UTIL_TUBE_TREE_H_

#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>
#include "tube_input.h"
#include "tube_node.h"

namespace util {

class TubeTree {
 public:
  TubeTree(unsigned bin_count) : bin_count_(bin_count){};

  TubeNode* root() {
    return root_.get();
  }

  // Constructs the tree based on |values|.
  void Build(std::vector<double> values);

  // Returns the discretized version of |feature_value|.
  unsigned Discretize(double feature_value) const;

  // Properly destroys the owned TUBE nodes.
  void Clear();

 private:
  // Fill the |discretized_value_| field (on util::TubeNode) for every leaf of
  // the tree.
  void SetupDiscretization();

  // The root of the TUBE tree.
  std::unique_ptr<TubeNode> root_;

  // The amount of bins we are going to use to break the range.
  unsigned bin_count_;

  // The object used to ease the splitting process.
  std::unique_ptr<TubeInput> tube_input_;
};

}  // namespace util

#endif
