// SmoteSegment is designed to correctly fracture the random segments selected
// by SMOTE and return the ::base::Document object associated with the best
// subsegment.
// For this class to work correctly, your data MUST BE NORMALIZED TO [0, 1].
#ifndef _RI_2015_1_KNN_SEGMENTED_SMOTE_SMOTE_SEGMENTER_H_
#define _RI_2015_1_KNN_SEGMENTED_SMOTE_SMOTE_SEGMENTER_H_

#include <cstdlib>
#include <iostream>
#include <queue>
#include <unordered_map>
#include "segmentation_metadata.h"
#include "../../base/document.h"
#include "../../util/discretizer.h"

namespace knn {

namespace segmented_smote {

class SmoteSegmenter {
 public:
  SmoteSegmenter(const ::util::Discretizer* discretizer,
                 const SegmentationMetadata* segmentation_metadata,
                 const ::base::Document& a, const ::base::Document& b);

  // Returns the ::base::Document object associated with the best fractured
  // segment according to MODAL, along with it's score.
  unsigned Segment(::base::Document& document);

  // Generates a synthetic sample between the points given by the parametric
  // values |lower| and |upper| and stores the resulting document in |document|.
  void GenerateSyntheticSample(double lower, double upper,
                               ::base::Document& document);

 private:
  // Constructs a ::base::Document object that lies between the values of
  // |segment_edges_| with parametric value |parameter|.
  void Rebuild(double parameter, ::base::Document& document);

  // Initializes the |segment_edges_| mapping.
  void Initialize(const ::base::Document& a, const ::base::Document& b);

  // Ensures the supplied |parameter| is valid. Returns true if it is, false
  // otherwise.
  bool ValidateParameter(double parameter);

  // Stores the sorted parameter separators of the segment we are fragmenting.
  void SeparatingParameters(std::vector<double>& parameters);

  // The mapping |feature_id| -> |interval limits| of the wanted line segment.
  std::unordered_map<unsigned, std::pair<double, double>> segment_edges_;

  // Object responsible for performing discretization operations.
  const ::util::Discretizer* discretizer_;

  // Support structure containing the countings necessary for fast MODAL
  // operations.
  const SegmentationMetadata* segmentation_metadata_;
};

}  // namespace segmented_smote
}  // namespace knn

#endif
