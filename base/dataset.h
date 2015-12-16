// base::Dataset is the last fundamental class of our library. It encapsulates
// the logic involved in the parsing and write processes, as well as keep a
// collection of Query objects in a more orderly manner. To use it, the most
// common case would be to construct the object and call the Parse method on a
// file storing the dataset you want to process.

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

  // Parses the file pointed by |file_path|. If |ordered_file|, assume the file
  // already has it's queries ordered.
  void Parse(const std::string& file_path, bool ordered_file = false);

  // Parses the file pointed by |file_path| and stores the results in
  // |queries_|. Assumes that queries are already ordered in the file.
  void OrderedParse(const std::string& file_path);

  // Reads the file pointed by |file_path| without assuming that queries are
  // already ordered. Stores the read objects in |query_|.
  void UnorderedParse(const std::string& file_path);

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
