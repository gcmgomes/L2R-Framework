#ifndef _RI_2015_1_BASE_QUERY_METADATA_H_
#define _RI_2015_1_BASE_QUERY_METADATA_H_

#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include "document.h"

namespace base {

class Query;

struct RelevanceEntry {
  RelevanceEntry() : count(0){};

  unsigned count;
};

class QueryMetadata {
 public:
  friend class Query;

  QueryMetadata();

 private:
  std::unordered_map<unsigned, RelevanceEntry> relevance_information_;
};

}  // namespace base

#endif
