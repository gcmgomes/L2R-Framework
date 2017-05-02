#include "ranker_helper.h"

#include "../../../bayes/branch_and_bound/variable.h"

namespace bayes {
namespace branch_and_bound {
  // Method that selects the instances of a given query.
  // If the query_number is negative, the entire set of instances is returned.
  std::vector<bayes::branch_and_bound::Instance> FilterInstances(
      const std::vector<bayes::branch_and_bound::Instance>& instances,
      int query_number)
  {
    if(query_number < 0) return instances;

    std::vector<bayes::branch_and_bound::Instance> filtered;
    for(bayes::branch_and_bound::Instance inst : instances) {
      if((int)inst.query_id() == query_number) {
        filtered.push_back(inst);
      }
    }
    return filtered;
  }
  
  // Ranks using a unique parent set...
  // This method returns a list of labels given to each document.
  std::vector<double> run(const bayes::branch_and_bound::Bitset& parent_set,
      const std::vector<bayes::branch_and_bound::Instance>& instances,
      const bayes::branch_and_bound::InvertedIndex& index,
      std::vector<bayes::branch_and_bound::Cache>& caches)
  {
    std::vector<bayes::branch_and_bound::Variable> variables;
    std::vector<bayes::branch_and_bound::inference::CPTable> cp_tables;
    cp_tables.push_back(
        bayes::branch_and_bound::inference::CPTable(0, parent_set));
    cp_tables[0].Build(index);
    bayes::branch_and_bound::Variable::InitializeVariables(index, variables,
        caches, cp_tables);
    bayes::branch_and_bound::Graph graph(variables);

    for(unsigned i = 0; i < parent_set.bit_count(); i++) {
      if(parent_set.at(i))
        graph.AddArc(i, 0, &index);
    }

    util::Discretizer disc(util::Discretizer::Mode::TREE_BASED_UNSUPERVISED,
        20);

    // Since we don't use the Rank function passing a document,
    // it doesn't matter the discretizer I pass in here.
    bayes::branch_and_bound::inference::Ranker ranker(graph, disc);

    // Keeps the ranking value for each instance. 
    std::vector<double> v_ranker;

    for(bayes::branch_and_bound::Instance inst : instances) {
      v_ranker.push_back(ranker.Rank(inst));
    }

    return v_ranker;
  }
  
  // Ranks using an ensemble of parent sets...
  // This method returns a list of labels given to each document.
  // The label given to each parent set is the mean of labels given per
  // parent set.
  std::vector<double> run(const std::vector<bayes::branch_and_bound::Bitset>&
      parent_sets,
      const std::vector<bayes::branch_and_bound::Instance>& instances,
      const bayes::branch_and_bound::InvertedIndex& index,
      std::vector<std::vector<bayes::branch_and_bound::Cache>*>& caches)
  {
    std::vector<double> v_ranker(instances.size(), 0);
    if(instances.size() == 0) return v_ranker;
    
    for(unsigned i = 0; i < parent_sets.size(); i++) {
      bayes::branch_and_bound::Bitset ps = parent_sets[i];
      std::vector<double> ps_ranked = run(ps, instances, index, *caches[i]);
      for(unsigned j = 0; j < v_ranker.size(); j++)
        v_ranker[j] += ps_ranked[j];
    }
    
    for(unsigned i = 0; i < v_ranker.size(); i++) {
      v_ranker[i] /= instances.size();
    }
    
    return v_ranker;
  }


} // namespace branch_and_bound
} // namespace bayes
