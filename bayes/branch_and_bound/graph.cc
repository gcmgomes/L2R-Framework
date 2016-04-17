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
                     const std::vector<std::vector<unsigned> >& topology,
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
  std::vector<std::vector<unsigned> > topology(variables_.size());
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

bool Graph::RemoveArc(unsigned parent_variable_id, unsigned child_variable_id) {
  if (!variables_[child_variable_id].parent_set().at(parent_variable_id) ||
      h_matrix_[parent_variable_id][child_variable_id] == ArcStatus::PRESENT) {
    return false;
  }
  Variable& child_var = variables_[child_variable_id];
  long double old_score = score_ - child_var.score();
  child_var.mutable_parent_set().Set(parent_variable_id, false);
  h_matrix_[parent_variable_id][child_variable_id] = ArcStatus::PROHIBITED;
  MakeCompliant(child_variable_id);
  score_ = old_score + child_var.score();
  return true;
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

void Graph::Initialize() {
  auto it = variables_.cbegin();
  h_matrix_.resize(variables_.size(),
                   std::vector<ArcStatus>(variables_.size(), ArcStatus::FREE));
  h_matrix_.shrink_to_fit();
  while (it != variables_.cend()) {
    if (it->categories().size() > 1) {
      score_ += it->score();
    }
    h_matrix_[it->variable_id()].shrink_to_fit();
    ++it;
  }
}

void Graph::MakeCompliant(unsigned child_variable_id) {
  Bitset prohibited_bits(variables_.size());
  unsigned parent_variable_id = 0;
  while (parent_variable_id < variables_.size()) {
    if (parent_variable_id != child_variable_id &&
        h_matrix_[parent_variable_id][child_variable_id] ==
            ArcStatus::PROHIBITED) {
      prohibited_bits.Set(parent_variable_id, true);
    }
    parent_variable_id++;
  }
  variables_[child_variable_id].mutable_parent_set() =
      variables_[child_variable_id].cache()->BestComplyingEntry(
          prohibited_bits);
}

}  // namespce branch_and_bound
}  // namespace bayes
