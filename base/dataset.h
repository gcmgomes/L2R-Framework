#ifndef _RI_2015_1_BASE_DATASET_H_
#define _RI_2015_1_BASE_DATASET_H_

#include <cstdlib>
#include <iostream>
#include <vector>
#include "query.h"

namespace base {

class Dataset {
 public:
  Dataset(){};

  const std::vector<::base::Query>& queries() {
    return queries_;
  }

  std::vector<::base::Query>& mutable_queries() {
    return queries_;
  }

  // Parses the file pointed to |file_path| and stores the results in |queries_|
  void Parse(const std::string& file_path);

  // Writes the contents of this dataset to |file_path|.
  void Write(const std::string& file_path);

  std::vector<::base::Query>::iterator begin() {
    return queries_.begin();
  }

  std::vector<::base::Query>::iterator end() {
    return queries_.end();
  }

  std::vector<::base::Query>::const_iterator cbegin() const {
    return queries_.cbegin();
  }

  std::vector<::base::Query>::const_iterator cend() const {
    return queries_.cend();
  }
  // Turns all queries into a single query, containing all documents in the
  // dataset. Only use this if you want to extract information pertaining to the
  // entire dataset, otherwise keep the queries separated.
  void Collapse(Query& query) const;

 private:
  std::vector<::base::Query> queries_;
};

}  // namespace base

#endif
