#include "query.h"

namespace base {

Query::Query(unsigned id, unsigned dimension_count) : id_(id), dimension_count_(dimension_count){}

void Query::AddDocument(const std::string& vector) {
  unsigned id = documents_.size();
  documents_.emplace_back(Document(id, vector, dimension_count_));
}

}
