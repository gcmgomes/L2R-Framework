#include <cmath>
#include "discretizer.h"
#include "tube_tree.h"

namespace util {

double Discretizer::Discretize(unsigned feature_id,
                               double feature_value) const {
  double value = -1;
  switch (mode_) {
    case Mode::UNIFORM_BIN_LENGTH:
      value = DiscretizeUniformBinLength(feature_value);
      break;
    case Mode::TREE_BASED_UNSUPERVISED:
      value = DiscretizeTube(feature_id, feature_value);
      break;
    default:
      break;
  }
  return value;
}

static void ExtractFeatureValues(const ::base::Query& query,
                                 unsigned feature_id,
                                 std::vector<double>& values) {
  auto document = query.cbegin();
  values.resize(query.size());
  unsigned i = 0;
  while (document != query.cend()) {
    if (document->vector().count(feature_id)) {
      values[i] = document->vector().at(feature_id);
    } else {
      values[i] = 0;
    }
    i++;
    ++document;
  }
}

void Discretizer::Initialize(const ::base::Query& query) {
  if (mode_ == Mode::UNIFORM_BIN_LENGTH) {
    std::vector<double> values;
    Initialize(0, values);
  } else if (mode_ == Mode::TREE_BASED_UNSUPERVISED) {
    auto feature_id = query.feature_set().cbegin();
    while (feature_id != query.feature_set().cend()) {
      std::cerr << "Discretizing feature " << *feature_id << std::endl;
      std::vector<double> feature_values;
      ExtractFeatureValues(query, *feature_id, feature_values);
      Initialize(*feature_id, feature_values);
      ++feature_id;
    }
  }
}

void Discretizer::Initialize(unsigned feature_id,
                             const std::vector<double>& values) {
  if (mode_ == Mode::UNIFORM_BIN_LENGTH) {
    double gap = 1.0 / (double)bin_count_;
    int i = 1;
    while (i <= bin_count_) {
      frontiers_[0].push_back(gap * (double)i);
      i++;
    }
  } else if (mode_ == Mode::TREE_BASED_UNSUPERVISED) {
    TubeTree tube_tree(bin_count_);
    tube_tree.Build(values);
    tube_tree.ListUpperIntervalLimits(frontiers_[feature_id]);
  }
}

double Discretizer::DiscretizeUniformBinLength(double feature_value) const {
  double b = bin_count_;
  if (feature_value >= 1) {
    return b - 1;
  }
  return floor(feature_value * b);
}

}  // namespace util
