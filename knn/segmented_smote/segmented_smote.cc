#include <random>
#include <chrono>
#include <ctime>
#include "segmented_smote.h"

namespace knn {

namespace segmented_smote {
SegmentedSmote::SegmentedSmote(const ::base::Query& sample_query,
                               unsigned relevance_level, unsigned k,
                               unsigned bin_count)
    : real_relevants_(sample_query.id(), sample_query.dimension_count()),
      synthetic_relevants_(sample_query.id(), sample_query.dimension_count()),
      relevance_level_(relevance_level), knn_(k), discretizer_(bin_count) {
  segmentation_metadata_ = SegmentationMetadata(&discretizer_, sample_query);
  Initialize(sample_query, bin_count);
};

bool SegmentedSmote::Initialize(const ::base::Query& query,
                                unsigned bin_count) {
  real_relevants_.mutable_documents().clear();
  synthetic_relevants_.mutable_documents().clear();

  auto doc = query.documents().begin();
  while (doc != query.documents().end()) {
    if (doc->relevance_label() == relevance_level_) {
      real_relevants_.AddDocument(doc->ToString());
    }
    ++doc;
  }

  if (real_relevants_.documents().size() < 2) {
    real_relevants_.mutable_documents().clear();
  } else {
    doc = real_relevants_.documents().begin();
    while (doc != real_relevants_.documents().end()) {
      knn_.UpdateNeighbours(real_relevants_, *doc);
      ++doc;
    }
  }

  return real_relevants_.documents().size() >= 2;
}

void SegmentedSmote::Populate(double extra_percentage, bool use_feedback) {
  unsigned i = 0;
  unsigned doc_id = 0, size = real_relevants_.documents().size();
  unsigned extra_count = ceil(size * (extra_percentage) / 100.0);
  auto doc = real_relevants_.documents().begin();
  while (i < extra_count) {
    while (!(rand() % 2)) {
      doc_id++;
      doc_id %= size;
    }
    auto doc = real_relevants_.documents().at(doc_id);
    unsigned nth_nn = rand() % knn_.neighbourhoods().at(doc.id()).size();
    unsigned nn_id = knn_.GetNthNeighbour(doc.id(), nth_nn).doc_id;
    GenerateSyntheticSample(doc, real_relevants_.documents()[nn_id]);
    std::cout << "\rQuery id: " << doc.query_id() << " Extras: " << i + 1;
    fflush(stdout);
    if (use_feedback) {
      real_relevants_.AddDocument(
          synthetic_relevants_.documents().back().ToString());
      UpdateNeighbourhoods(size, std::make_pair(doc_id, nn_id));
    }
    ++i;
  }
  std::cout << std::endl;
}

void SegmentedSmote::AugmentQuery(::base::Query& query) {
  auto doc = synthetic_relevants_.begin();
  while (doc != synthetic_relevants_.end()) {
    query.AddDocument(doc->ToString());
    ++doc;
  }
}

void SegmentedSmote::GenerateSyntheticSample(const ::base::Document& d1,
                                             const ::base::Document& d2) {
  synthetic_relevants_.mutable_documents().emplace_back(
      ::base::Document(synthetic_relevants_.documents().size()));

  smote_segmenter_.reset(
      new SmoteSegmenter(&discretizer_, &segmentation_metadata_, d1, d2));

  smote_segmenter_->Segment(synthetic_relevants_.mutable_documents().back());

  char meta[128];
  sprintf(meta, "#synthetic = %lu, (%u) -- (%u)",
          synthetic_relevants_.mutable_documents().size(), d1.id(), d2.id());
  synthetic_relevants_.mutable_documents().back().mutable_meta_information() =
      meta;

  synthetic_relevants_.mutable_documents().back().mutable_query_id() =
      d1.query_id();

  synthetic_relevants_.mutable_documents().back().mutable_relevance_label() =
      relevance_level_;
  
  this->segmentation_metadata_.Increment(synthetic_relevants_.documents().back());
}

void SegmentedSmote::UpdateNeighbourhoods(
    unsigned real_size, std::pair<unsigned, unsigned> execeptions) {
  auto synthetic = synthetic_relevants_.documents().back();
  unsigned i = 0;
  while (i < real_size) {
    auto base = real_relevants_.documents().at(i);
    knn_.UpdateNeighbour(base, synthetic);
    ++i;
  }
}

}  // namespce segmented_smote
}  // namespce knn
