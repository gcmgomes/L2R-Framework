#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <sstream>
#include "instance.h"

namespace bayes {
namespace branch_and_bound {

Instance::Instance() {
}

Instance::Instance(const base::Document& document,
                   const util::Discretizer& discretizer) {
  Initialize(document, discretizer);
}

void Instance::Initialize(const base::Document& document,
                          const util::Discretizer& discretizer) {
  values_.resize(document.vector().size()+1, 0);
  values_[0] = document.relevance_label();
  auto feature_it = document.cbegin();
  while (feature_it != document.cend()) {
    values_[feature_it->first] =
        discretizer.Discretize(feature_it->first, feature_it->second);
    ++feature_it;
  }
  values_.shrink_to_fit();
}

void Instance::ParseDataset(const std::string& file_path,
                            const util::Discretizer& discretizer,
                            std::vector<Instance>& instances) {
  util::Discretizer disc(discretizer.mode(), discretizer.bin_count());
  base::Dataset* dataset = new base::Dataset();
  dataset->Parse(file_path);
  base::Query query(1, dataset->cbegin()->dimension_count());
  dataset->Collapse(query);
  delete dataset;
  disc.Initialize(query);
  auto document = query.cbegin();
  while (document != query.cend()) {
    instances.emplace_back(*document, disc);
    ++document;
  }
}

std::string Instance::ToString() const {
  unsigned i = 1;
  std::stringstream str;
  str << "L:" << values_.at(0);
  while (i < values_.size()) {
      str << " " << i << ":" << values_.at(i);
    i++;
  }
  return str.str();
}

}  // namespace branch_and_bound
}  // namespace bayes
