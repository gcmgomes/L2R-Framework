// SegmentationMetadata is an utility class to be used alongside SegmentedSmote.
// It holds a few fileds to speed up computations related to the usage o MODAL
// within Smote. Refer to the individual attributes and methods for further
// information.
// For this class to work correctly, your data MUST BE NORMALIZED TO [0, 1].
#ifndef _RI_2015_1_KNN_SEGMENTED_SMOTE_SEGMENTATION_METADATA_H_
#define _RI_2015_1_KNN_SEGMENTED_SMOTE_SEGMENTATION_METADATA_H_

#include <cstdlib>
#include <iostream>
#include <queue>
#include <unordered_map>
#include "../../util/discretizer.h"
#include "../../base/query.h"

namespace knn {

namespace segmented_smote {

class SegmentationMetadata {
 public:
  SegmentationMetadata(){};
  SegmentationMetadata(const ::util::Discretizer* discretize,
                       const ::base::Query& query);

  const std::unordered_map<unsigned, std::unordered_map<unsigned, unsigned>>&
  feature_value_map() const {
    return feature_value_map_;
  }

  std::unordered_map<unsigned, std::unordered_map<unsigned, unsigned>>&
  mutable_feature_value_map() {
    return feature_value_map_;
  }

  // Increments the appropriate mapping counts based on the feature values of
  // |document|.
  void Increment(const ::base::Document& document);

  // Increments the appropriate mapping count based on |feature_id| and
  // |feature_value|.
  void Increment(unsigned feature_id, double feature_value);

  // Returns the count indexed by |feature_id| and |feature_value|.
  unsigned Access(unsigned feature_id, double feature_value) const;

  // Performs the the MODAL scoring of document |d|.
  unsigned Score(const ::base::Document& d) const;

 private:
  // Initializes the needed fields for operation.
  void Initialize(const ::base::Query& query);

  // Object responsible for performing discretization operations.
  const ::util::Discretizer* discretizer_;

  // The mapping |feature_id| x |feature_value| -> count.
  std::unordered_map<unsigned, std::unordered_map<unsigned, unsigned>>
      feature_value_map_;
};

}  // namespace segmented_smote
}  // namespace knn

#endif
