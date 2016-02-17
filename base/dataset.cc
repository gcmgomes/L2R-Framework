#include <iostream>
#include <fstream>
#include "dataset.h"

namespace base {

// Returns the initial query_id and hash size needed for this dataset.
static std::pair<std::string, unsigned> GetStartingData(
    const std::string& file_path) {
  std::string vector;
  std::ifstream input_file(file_path.c_str());
  std::getline(input_file, vector);
  Document d(0, vector);
  std::string qid = Document::GetQueryId(vector);
  return std::make_pair(qid, d.vector().size());
}

void Dataset::Parse(const std::string& file_path) {
  std::unordered_map<std::string, unsigned> query_mapping;
  unsigned query_id = 0, dimension_count = 0;
  std::pair<std::string, unsigned> data = GetStartingData(file_path);
  query_mapping[data.first] = query_mapping.size() + 1;
  query_id = query_mapping[data.first];
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
    std::string temp_qid = Document::GetQueryId(current_vector);
    if (!query_mapping.count(temp_qid)) {
      query_id = query_mapping.size() + 1;
      query_mapping[temp_qid] = query_id;
      temporary_vector = current_vector;
      queries_.back().mutable_documents().pop_back();
      queries_.emplace_back(Query(query_id, dimension_count));
    } else {
      queries_.back().mutable_documents().back().mutable_query_id() = query_id;
    }
  }
}

void Dataset::Write(const std::string& file_path) {
  std::ofstream file(file_path.c_str());
  auto query = queries_.begin();
  while (query != queries_.end()) {
    auto doc = query->begin();
    while (doc != query->end()) {
      file << doc->ToString() << std::endl;
      ++doc;
    }
    ++query;
  }
}

void Dataset::Collapse(Query& query, bool modify_qid) const {
  auto query_it = queries_.cbegin();
  while (query_it != queries_.cend()) {
    auto doc_it = query_it->cbegin();
    while (doc_it != query_it->cend()) {
      unsigned query_id = (modify_qid) ? 1 : doc_it->query_id();
      query.AddDocument(doc_it->ToString());
      query.mutable_documents().back().mutable_query_id() = query_id;
      ++doc_it;
    }
    ++query_it;
  }
}

}  // namespace base
