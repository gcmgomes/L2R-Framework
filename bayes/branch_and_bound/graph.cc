#include <algorithm>
#include <limits>
#include <sstream>
#include "graph.h"

namespace bayes {
namespace branch_and_bound {

Graph::Graph() : score_(std::numeric_limits<int>::min()) {
}

Graph::Graph(long double score) : score_(score) {
}

Graph::Graph(const Graph& other) {
  score_ = other.score();
  variables_ = other.variables();
  h_matrix_ = other.h_matrix();
}

Graph::Graph(const std::vector<Variable>& variables) {
  score_ = 0.0;
  Initialize(variables);
}

static void CycleDFS(unsigned search_root,
                     const std::vector<std::vector<unsigned>>& topology,
                     std::vector<unsigned>& visited,
                     std::vector<unsigned>& cycle) {
  if (visited[search_root] == 1) {  // |search_root| is open, found a cycle!
    cycle.push_back(search_root);
    return;
  }
  if (visited[search_root] == 2) {
    return;
  }
  visited[search_root] = 1;
  auto v = topology[search_root].cbegin();
  while (v != topology[search_root].cend()) {
    CycleDFS(*v, topology, visited, cycle);
    if (!cycle.empty()) {
      if (!(cycle.size() > 1 && cycle.front() == cycle.back())) {
        cycle.push_back(search_root);
      }
      break;
    }
    ++v;
  }
  visited[search_root] = 2;
}

void Graph::FindCycle(std::vector<unsigned>& cycle) const {
  std::vector<std::vector<unsigned>> topology(variables_.size());
  auto it = variables_.cbegin();
  while (it != variables_.cend()) {
    auto high_bits = it->parent_set().high_bits();
    auto u = high_bits.cbegin();
    while (u != high_bits.cend()) {
      topology[*u].push_back(it->variable_id());
      ++u;
    }
    ++it;
  }
  std::vector<unsigned> visited(variables_.size(), 0);
  unsigned node = 0;
  while (node < topology.size()) {
    if (!visited[node]) {
      CycleDFS(node, topology, visited, cycle);
      if (!cycle.empty()) {
        std::reverse(cycle.begin(), cycle.end());
        break;
      }
    }
    node++;
  }
}

static bool GoodToGo(
    std::unordered_map<unsigned, std::unordered_map<unsigned, ArcStatus>>
        h_matrix,
    unsigned parent_variable_id, unsigned child_variable_id, ArcStatus status) {
  return (h_matrix.count(parent_variable_id) &&
          h_matrix.at(parent_variable_id).count(child_variable_id) &&
          h_matrix.at(parent_variable_id).at(child_variable_id) == status);
}

bool Graph::RemoveArc(unsigned parent_variable_id, unsigned child_variable_id) {
  if (GoodToGo(h_matrix_, parent_variable_id, child_variable_id,
               ArcStatus::PRESENT)) {
    return false;
  }
  Variable& child_var = variables_[child_variable_id];
  long double old_score = score_ - child_var.score();
  child_var.mutable_parent_set().Set(parent_variable_id, false);
  h_matrix_[parent_variable_id][child_variable_id] = ArcStatus::PROHIBITED;
  if (BestCompliantEntry(child_variable_id)) {
    score_ = old_score + child_var.score();
    return true;
  }
  return false;
}

std::string Graph::ToString(std::string left_padding) const {
  std::stringstream str;
  auto it = variables_.cbegin();
  while (it != variables_.cend()) {
    str << left_padding << "Variable " << it->variable_id()
        << ": Parents: " << it->parent_set().ToString()
        << " Score:" << it->score() << std::endl;
    ++it;
  }
  str << left_padding << "Graph score: " << score() << std::endl;
  return str.str();
}

bool Graph::ReadyForUse(const std::vector<Variable>& variables) {
  score_ = 0.0;
  variables_ = variables;
  auto it = variables_.begin();
  while (it != variables_.end()) {
    if (!BestCompliantEntry(it->variable_id())) {
      return false;
    }
    ++it;
  }
  Initialize(variables_);
  return true;
}

void Graph::RecomputeScore() {
  score_ = 0;
  Initialize(variables_);
}

void Graph::Initialize(const std::vector<Variable>& variables) {
  auto it = variables.cbegin();
  while (it != variables.cend()) {
    if (it->categories().size() > 1) {
      score_ += it->score();
    }
    ++it;
  }
}

bool Graph::BestCompliantEntry(unsigned child_variable_id) {
  if(variables_[child_variable_id].cache()->cache().empty()) {
    return true;    
  }
  Bitset prohibited_bits(variables_.size());
  Bitset mandatory_bits(variables_.size());
  unsigned parent_variable_id = 0;
  while (parent_variable_id < variables_.size()) {
    if (parent_variable_id != child_variable_id) {
      if (GoodToGo(h_matrix_, parent_variable_id, child_variable_id,
                   ArcStatus::PROHIBITED)) {
        prohibited_bits.Set(parent_variable_id, true);
      } else if (GoodToGo(h_matrix_, parent_variable_id, child_variable_id,
                          ArcStatus::PRESENT)) {
        mandatory_bits.Set(parent_variable_id, true);
      }
    }
    parent_variable_id++;
  }
  Bitset best_complying_entry;
  if (variables_[child_variable_id].cache()->BestCompliantEntry(
          prohibited_bits, mandatory_bits, best_complying_entry)) {
    variables_[child_variable_id].mutable_parent_set() = best_complying_entry;
    return true;
  }
  return false;
}

}  // namespce branch_and_bound
}  // namespace bayes
