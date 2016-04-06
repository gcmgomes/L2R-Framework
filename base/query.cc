#include "query.h"

namespace base {

Query::Query(unsigned id, unsigned dimension_count)
    : id_(id), dimension_count_(dimension_count) {
}

void Query::AddDocument(const std::string& vector) {
  unsigned id = documents_.size();
  documents_.emplace_back(Document(id, vector, dimension_count_));
  documents_.back().GetKnownFeatures(feature_set_);
}

void Query::AddDocument(const Document& document) {
  unsigned id = documents_.size();
  documents_.push_back(document);
  documents_.back().mutable_id() = id;
  documents_.back().GetKnownFeatures(feature_set_);
}

}  // namespace base
