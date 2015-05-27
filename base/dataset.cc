#include <iostream>
#include <fstream>
#include "dataset.h"

namespace base {

// Returns the initial query_id and hash size needed for this dataset.
static std::pair<unsigned, unsigned> GetStartingData(
    const std::string& file_path) {
  std::string vector;
  std::ifstream input_file(file_path.c_str());
  std::getline(input_file, vector);
  Document d(0, vector);
  return std::make_pair(d.query_id(), d.vector().size());
}

void Dataset::Parse(const std::string& file_path) {
  unsigned query_id = 0, dimension_count = 0;
  std::pair<unsigned, unsigned> data = GetStartingData(file_path);
  query_id = data.first;
  dimension_count = data.second;
  std::ifstream input_file;
  input_file.open(file_path.c_str(), std::ifstream::in);
  queries_.emplace_back(Query(query_id, dimension_count));
  std::string temporary_vector("");

  while ((!input_file.eof() && input_file.peek() != EOF) ||
         !temporary_vector.empty()) {
    std::string current_vector;
    if (!temporary_vector.empty()) {
      current_vector = temporary_vector;
      temporary_vector.clear();
    } else {
      getline(input_file, current_vector);
    }
    queries_.back().AddDocument(current_vector);
    if (queries_.back().documents().back().query_id() != query_id) {
      query_id = queries_.back().documents().back().query_id();
      temporary_vector = current_vector;
      queries_.back().mutable_documents().pop_back();
      queries_.emplace_back(Query(query_id, dimension_count));
    }
  }
}

}  // namespace base
