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

 private:
  unsigned id_;

  unsigned dimension_count_;

  // Set of documents present in query.
  std::vector<Document> documents_;
};

}  // namespace base

#endif
