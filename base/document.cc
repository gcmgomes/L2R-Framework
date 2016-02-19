#include <map>
#include <vector>
#include <string>
#include <cstdio>
#include <cmath>
#include <limits>
#include <sstream>
#include "document.h"

namespace base {

Document::Document(unsigned id, const std::string& vector, unsigned hash_size) {
  id_ = id;
  query_id_ = -1;  // Serves to indicate that we don't have an associated query.
                   // Easier to debug.
  this->vector_.reserve(hash_size);
  relevance_label_ = 0;
  if (!vector.empty()) {
    Parse(vector);
  }
}

Document::Document(unsigned id, const std::string& vector)
    : Document(id, vector, 0){};

Document::Document(unsigned id) : Document(id, ""){};

void Document::Parse(const std::string& vector) {
  size_t meta_begin = vector.rfind("#");
  if (meta_begin != std::string::npos) {
    this->meta_information_ = vector.substr(meta_begin);
  }
  std::stringstream vector_stream(vector.substr(0, meta_begin));
  vector_stream >> this->relevance_label_;
  vector_stream.ignore(std::numeric_limits<std::streamsize>::max(), ':');
  vector_stream.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
  while (!vector_stream.eof()) {
    int feature_id = 0;
    double feature_value = 0;
    vector_stream >> feature_id;
    vector_stream.ignore(std::numeric_limits<std::streamsize>::max(), ':');
    vector_stream >> feature_value;
    this->InsertDimension(feature_id, feature_value);
  }
}

// Change this to use StatusOr as soon as I know how to use templates.
double& Document::operator[](unsigned dimension) {
  return vector_[dimension];
}

void Document::InsertDimension(unsigned dimension, double value) {
  vector_[dimension] = value;
}

std::string Document::GetQueryId(const std::string& vector) {
  std::string query_id(128, 0);
  sscanf(vector.c_str(), "%*d qid:%s", &query_id[0]);
  return query_id;
}

void Document::GetKnownFeatures(
    std::unordered_set<unsigned>& feature_set) const {
  auto id = this->cbegin();
  while (id != this->cend()) {
    feature_set.insert(id->first);
    ++id;
  }
}

std::string Document::ToString() const {
  std::string str;
  char i[128];
  sprintf(i, "%u qid:%u", relevance_label_, query_id_);
  str += i;
  std::map<unsigned, double> sorted_vector;
  auto k = vector_.begin();
  while (k != vector_.end()) {
    sorted_vector[k->first] = k->second;
    ++k;
  }
  auto j = sorted_vector.begin();
  while (j != sorted_vector.end()) {
    sprintf(i, " %u:%lf", j->first, j->second);
    str += i;
    ++j;
  }

  if (!meta_information_.empty()) {
    str += ' ';
    str += meta_information_;
  }
  return str;
}

}  // namespace base
