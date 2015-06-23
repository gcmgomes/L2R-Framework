#ifndef _RI_2015_1_BASE_DOCUMENT_H_
#define _RI_2015_1_BASE_DOCUMENT_H_

#include <cstdlib>
#include <iostream>
#include <unordered_map>

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

  std::string ToString() const;

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
