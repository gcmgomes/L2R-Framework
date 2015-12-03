#include <cstdlib>
#include <iostream>
#include <queue>
#include <unordered_map>
#include "segmentation_metadata.h"

namespace knn {

namespace segmented_smote {

SegmentationMetadata::SegmentationMetadata(
    const ::util::Discretizer* discretizer, const ::base::Query& query) {
  discretizer_ = discretizer;
  Initialize(query);
}

void SegmentationMetadata::Increment(const ::base::Document& document) {
  auto feature = document.cbegin();
  while(feature != document.cend()) {
    this->Increment(feature->first, feature->second);
    ++feature;
  }

}


void SegmentationMetadata::Increment(unsigned feature_id,
                                     double feature_value) {
  unsigned discrete_value = discretizer_->Discretize(feature_id, feature_value);
  feature_value_map_[feature_id][discrete_value]++;
}

unsigned SegmentationMetadata::Access(unsigned feature_id,
                                      double feature_value) const {
  unsigned key = discretizer_->Discretize(feature_id, feature_value);
  if (feature_value_map().count(feature_id) &&
      feature_value_map().at(feature_id).count(key)) {
    return feature_value_map().at(feature_id).at(key);
  }
  return 0;
}

unsigned SegmentationMetadata::Score(const ::base::Document& d) const {
  auto feature = d.cbegin();
  unsigned score = 0;
  while (feature != d.cend()) {
    if (feature_value_map_.count(feature->first)) {
      unsigned key = discretizer_->Discretize(feature->first, feature->second);
      const auto& value_map = feature_value_map_.at(feature->first);
      if (value_map.count(key)) {
        score += value_map.at(key);
      }
    }
    ++feature;
  }
  return score;
}

void SegmentationMetadata::Initialize(const ::base::Query& query) {
  feature_value_map_.clear();
  auto d = query.cbegin();
  while (d != query.cend()) {
    Increment(*d);
    ++d;
  }
}

}  // namespace segmented_smote
}  // namespace knn
