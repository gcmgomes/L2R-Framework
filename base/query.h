// Much like base::Document, base::Query is another important building block for
// Learning to Rank. Unlike classification, our data points are "clustered" in
// comparable bundles. For instance, we cannot directly compare a base::Document
// object coming from a query A to another of query B. There is no meaning to
// such comparison. As to the implementaion, we here opt to use traditional
// std::vectors. They go nicely with the concept of document identifiers and
// usually will be no longer than some thousands of points. We also keep track
// of the amount of features (dimensions) we have stored here. These 4 bytes
// help when constructing a base::Document object, as we can avoid the need of
// reallocating the hash table, we know in advance how large it can be.

#ifndef _RI_2015_1_BASE_QUERY_H_
#define _RI_2015_1_BASE_QUERY_H_

#include <cstdlib>
#include <iostream>
#include <vector>
#include "document.h"

namespace base {

class Query {
 public:
  Query(unsigned id, unsigned dimension_size);

  // Constructs and inserts a base::Document object to the end of the current
  // document vector.
  void AddDocument(const std::string& vector);

  unsigned dimension_count() const {
    return dimension_count_;
  }

  unsigned& mutable_dimension_count() {
    return dimension_count_;
  }

  unsigned id() const {
    return id_;
  }

  unsigned& mutable_id() {
    return id_;
  }

  const std::vector<Document>& documents() const {
    return documents_;
  }

  std::vector<Document>& mutable_documents() {
    return documents_;
  }

  std::vector<Document>::const_iterator cbegin() const {
    return documents_.cbegin();
  }

  std::vector<Document>::const_iterator cend() const {
    return documents_.cend();
  }

  std::vector<Document>::iterator begin() {
    return documents_.begin();
  }

  std::vector<Document>::iterator end() {
    return documents_.end();
  }

 private:
  unsigned id_;

  unsigned dimension_count_;

  // Set of documents present in query.
  std::vector<Document> documents_;
};

}  // namespace base

#endif
