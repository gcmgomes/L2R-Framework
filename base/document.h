// base::Document is one of the most fundamental structures of this library. It
// handles the representation of the smallest building block of a standard
// Learning to Rank environment, the high dimensional feature vectors. This
// class is essentially a wrapper over C++11's hash tables (std::unordered_map),
// with some additional functionalities and helpers, such as the Parse and
// ToString methods, respectively.
// We use hash tables instead of simpler vectors because we might have to work
// with sparse data. In the future, we might consider using serialization
// libraries
// such as Google's Protocol Buffer to allow some portability across languages.
// Another possible optimization (in terms of space), would be to keep pointers
// to the locations of the vector in the dataset files. If we reach the point of
// worrying about memory, this feature will be implemented.

#ifndef _RI_2015_1_BASE_DOCUMENT_H_
#define _RI_2015_1_BASE_DOCUMENT_H_

#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

namespace base {

class Document {
 public:
  // Builds the object using |vector| as its representation and the amount of
  // available buckets to the hash container as |hash_size|
  Document(unsigned id, const std::string& vector, unsigned hash_size);

  // Same as above, but does not perform the prealloc of hash buckets.
  Document(unsigned id, const std::string& vector);

  // Builds a Document obkect with only the id field set.
  Document(unsigned id);

  // Parses the document encoded in |vector| into the internal document
  // representation.
  void Parse(const std::string& vector);

  // Returns the value of the document with respect to the supplied |dimension|.
  // Does not increases the dimensional count of the document.
  double& operator[](unsigned dimension);

  const std::unordered_map<unsigned, double>& vector() const {
    return vector_;
  }

  std::unordered_map<unsigned, double>& mutable_vector() {
    return vector_;
  }

  const std::string& meta_information() const {
    return meta_information_;
  }

  std::string& mutable_meta_information() {
    return meta_information_;
  }

  const unsigned& id() const {
    return id_;
  }

  unsigned& mutable_id() {
    return id_;
  }

  const unsigned& query_id() const {
    return query_id_;
  }

  unsigned& mutable_query_id() {
    return query_id_;
  }

  const unsigned& relevance_label() const {
    return relevance_label_;
  }

  unsigned& mutable_relevance_label() {
    return relevance_label_;
  }

  const double& length() const {
    return length_;
  }

  double& mutable_length() {
    return length_;
  }

  // Increases the dimensional count of the document by appending |dimension| to
  // its list of dimensions and sets its value to |value|. Also, it does not
  // update the values, only inserts brand new ones.
  void InsertDimension(unsigned dimesion, double value);

  // Returns the id of the query to which |vector| belongs.
  static std::string GetQueryId(const std::string& vector);

  // Adds the known feature of |this| to |feature_ids|.
  void GetKnownFeatures(std::unordered_set<unsigned>& feature_set) const;

  std::string ToString() const;

  std::unordered_map<unsigned, double>::const_iterator cbegin() const {
    return vector_.cbegin();
  }

  std::unordered_map<unsigned, double>::const_iterator cend() const {
    return vector_.cend();
  }

  std::unordered_map<unsigned, double>::iterator begin() {
    return vector_.begin();
  }

  std::unordered_map<unsigned, double>::iterator end() {
    return vector_.end();
  }

 private:
  unsigned id_;

  unsigned query_id_;

  std::unordered_map<unsigned, double> vector_;

  unsigned relevance_label_;

  double length_;

  std::string meta_information_;
};

}  // namespace base

#endif
