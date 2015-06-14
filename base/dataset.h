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

  // Parses the file pointed to |file_path| and stores the results in |queries_|
  void Parse(const std::string& file_path);

  std::vector<::base::Query>::iterator begin() {
    return queries_.begin();
  }

  std::vector<::base::Query>::iterator end() {
    return queries_.end();
  }

 private:
  std::vector<::base::Query> queries_;
};

}  // namespace base

#endif
