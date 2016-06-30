// This class reprents a discretized point of the desired euclidean space. The
// best way to build this class is to first use the functionalities built into
// base::Dataset, base::Query and util::Discretizer. Afterwards, simply assemble
// this object appropriately. Since we are interested in dense datasets, we will
// remove the overhead associated with hash tables.
//
// ATTENTION: values_[0] is reserved for the label of the document.
#ifndef _RI_2015_1_BAYES_BRANCH_AND_BOUND_INSTANCE_H_
#define _RI_2015_1_BAYES_BRANCH_AND_BOUND_INSTANCE_H_

#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include "../../base/dataset.h"
#include "../../util/discretizer.h"

namespace bayes {
namespace branch_and_bound {

class Instance {
 public:
  Instance();
  Instance(const base::Document& document,
           const util::Discretizer& discretizer);

  Instance(const std::string& instance_string);

  const std::vector<unsigned>& values() const {
    return values_;
  }

  std::vector<unsigned>& mutable_values() {
    return values_;
  }

  static void ParseDataset(const std::string& file_path,
                           util::Discretizer& discretizer,
                           std::vector<Instance>& instances);

  static void ParseDataset(const std::string& file_path,
                           std::vector<Instance>& instances);

  std::string ToString() const;

 private:
  // Performs initialization of |this| using |document| as the data source and
  // |discretizer| to perform the appropriate transformation.
  void Initialize(const base::Document& document,
                  const util::Discretizer& discretizer);

  // Parses |înstance_string|.
  void Initialize(const std::string& instance_string);

  std::vector<unsigned> values_;
};

}  // namespce branch_and_bound
}  // namespace bayes

#endif
