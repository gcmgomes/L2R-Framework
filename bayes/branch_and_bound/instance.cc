#include "instance.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include "../../util/normalizer.h"

namespace bayes {
namespace branch_and_bound {

Instance::Instance() {
  query_id_ = -1;
  doc_id_ = -1;
}

Instance::Instance(const base::Document& document,
                   const util::Discretizer& discretizer) {
  Initialize(document, discretizer);
}

Instance::Instance(const std::string& instance_string) {
  Initialize(instance_string);
}

Instance::Instance(const base::Document& document,
                   const util::Discretizer& discretizer,
                   const unsigned& query_id, const unsigned& doc_id) {
  Initialize(document, discretizer);
  query_id_ = query_id;
  doc_id_ = doc_id;
}

void Instance::Initialize(const base::Document& document,
                          const util::Discretizer& discretizer) {
  query_id_ = -1;
  doc_id_ = -1;
  values_.resize(document.vector().size() + 1, 0);
  values_[0] = document.relevance_label();
  auto feature_it = document.cbegin();
  while (feature_it != document.cend()) {
    values_[feature_it->first] =
        discretizer.Discretize(feature_it->first, feature_it->second);
    ++feature_it;
  }
  values_.shrink_to_fit();
}

void Instance::Initialize(const std::string& instance_string) {
  std::istringstream stream(instance_string);
  int cnt = -1;
  query_id_ = -1;
  doc_id_ = -1;
  
  do {
    std::string str_value;
    unsigned value=-1;
    stream >> str_value;
    cnt++;

    if(cnt==0)
    {
      sscanf(str_value.c_str(), "%u", &value);
    }
    else if(cnt==1)
    {
      sscanf(str_value.c_str(), "qid:%u", &value);
      query_id_ = value;
      continue;
    }
    else
    {
      unsigned tmp;
      if(str_value.find(":") != std::string::npos)
        sscanf(str_value.c_str(), "%u:%u", &tmp, &value);
      else continue;
    }
    
    values_.push_back(value);
  } while(stream);
}

// Returns true if we are handling a matrix dataset.
static bool ParserType(const std::string& file_path) {
  std::ifstream file(file_path);
  std::string str;
  getline(file, str);
  return str.find("qid") == std::string::npos;
}

void Instance::ParseDataset(const std::string& file_path,
                            util::Discretizer& discretizer,
                            std::vector<Instance>& instances) {
  if (ParserType(file_path)) {
    ParseDataset(file_path, instances);
    return;
  }
  util::Discretizer& disc = discretizer;
  base::Dataset* dataset = new base::Dataset();
  dataset->Parse(file_path);
  base::Query query(1, dataset->cbegin()->dimension_count());
  dataset->Collapse(query, false);
  util::Normalizer norm;
  norm.Normalize(util::Normalizer::Mode::FEATURE_SCALING, query);
  delete dataset;
  disc.Initialize(query);
  auto document = query.cbegin();
  while (document != query.cend()) {
    instances.emplace_back(*document, disc, document->query_id(),
                           document->id());
    ++document;
  }
  instances.shrink_to_fit();
}

void Instance::ParseDataset(const std::string& file_path,
                            std::vector<Instance>& instances) {
  std::ifstream input_file(file_path);
  while (!input_file.eof()) {
    std::string instance_string;
    getline(input_file, instance_string);
    if (input_file.eof()) {
      break;
    }
    instances.emplace_back(instance_string);
  }
  instances.shrink_to_fit();
}

std::string Instance::ToString() const {
  unsigned i = 1;
  std::stringstream str;
  str << values_.at(0);
  while (i < values_.size()) {
    str << " " << i << ":" << values_.at(i);
    i++;
  }
  return str.str();
}

std::string Instance::ToPrintableString() const {
  unsigned i = 1;
  std::stringstream str;
  str << values_.at(0);
  str << " qid:" << query_id_;
  while (i < values_.size()) {
    str << " " << i << ":" << values_.at(i);
    i++;
  }
  str << " #id" << doc_id_;
  return str.str();
}

}  // namespace branch_and_bound
}  // namespace bayes
