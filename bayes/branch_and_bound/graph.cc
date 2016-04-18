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
  variables_ = variables;
  score_ = 0.0;
  Initialize();
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
  if (!variables_[child_variable_id].parent_set().at(parent_variable_id) ||
      GoodToGo(h_matrix_, parent_variable_id, child_variable_id,
               ArcStatus::PRESENT)) {
    return false;
  }
  Variable& child_var = variables_[child_variable_id];
  long double old_score = score_ - child_var.score();
  child_var.mutable_parent_set().Set(parent_variable_id, false);
  h_matrix_[parent_variable_id][child_variable_id] = ArcStatus::PROHIBITED;
  if(MakeCompliant(child_variable_id)) {
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
  str << left_padding << "H Matrix:" << std::endl;
  unsigned line = 0;
  while (line < h_matrix_.size()) {
    unsigned column = 0;
    str << left_padding;
    while (column < h_matrix_.at(line).size()) {
      if (column) {
        str << " ";
      }
      unsigned x = static_cast<unsigned>(h_matrix_.at(line).at(column));
      str << x;
      column++;
    }
    str << std::endl;
    line++;
  }
  return str.str();
}

Bitset Graph::Hashable() const {
  Bitset bitset(variables_.size() * variables_.size());
  auto variable = variables_.cbegin();
  unsigned padding = 0;
  while(variable != variables_.cend()) {
    auto high_bits = variable->parent_set().high_bits();
    auto bit = high_bits.begin();
    while(bit != high_bits.end()) {
      bitset.Set(*bit + padding, true);
      ++bit;    
    }
    padding += variables_.size();
    ++variable;
  }
  return bitset;
}

void Graph::Initialize() {
  auto it = variables_.cbegin();
  while (it != variables_.cend()) {
    if (it->categories().size() > 1) {
      score_ += it->score();
    }
    ++it;
  }
}

bool Graph::MakeCompliant(unsigned child_variable_id) {
  Bitset prohibited_bits(variables_.size());
  Bitset mandatory_bits(variables_.size());
  unsigned parent_variable_id = 0;
  while (parent_variable_id < variables_.size()) {
    if (parent_variable_id != child_variable_id) {
      if (GoodToGo(h_matrix_, parent_variable_id, child_variable_id,
                   ArcStatus::PROHIBITED)) {
        prohibited_bits.Set(parent_variable_id, true);
      }
      else if(GoodToGo(h_matrix_, parent_variable_id, child_variable_id,
                   ArcStatus::PRESENT)) {
        mandatory_bits.Set(parent_variable_id, true);
      }
    }
    parent_variable_id++;
  }
  Bitset best_complying_entry;
  if(variables_[child_variable_id].cache()->BestComplyingEntry(
      prohibited_bits, mandatory_bits, best_complying_entry)) {
    variables_[child_variable_id].mutable_parent_set() = best_complying_entry;
    return true;
  }
  return false;
}

}  // namespce branch_and_bound
}  // namespace bayes
