#include <algorithm>
#include "k_nearest_neighbours.h"
#include "../euclid/vector_operations.h"

namespace knn {

void KNearestNeighbours::UpdateNeighbour(
    const ::base::Document& base_document,
    const ::base::Document& updating_document) {
  unsigned base_id = base_document.id(), update_id = updating_document.id();
  double distance =
      ::euclid::EuclideanDistance(base_document, updating_document);
  if (!neighbourhoods_.count(base_id) || neighbourhoods_[base_id].size() < k_) {
    neighbourhoods_[base_id].emplace(NeighbourhoodEntry(update_id, distance));
  } else {
    const auto& top = neighbourhoods_[base_id].top();
    if (top.distance > distance) {
      neighbourhoods_[base_id].pop();
      neighbourhoods_[base_id].emplace(NeighbourhoodEntry(update_id, distance));
    }
  }
}

void KNearestNeighbours::UpdateNeighbours(const ::base::Query& query,
                                          const ::base::Document& document) {
  auto current_doc = query.documents().begin();
  while (current_doc != query.documents().end()) {
    if (current_doc->id() != document.id()) {
      UpdateNeighbour(*current_doc, document);
    }
    ++current_doc;
  }
}

NeighbourhoodEntry KNearestNeighbours::GetNthNeighbour(unsigned id, unsigned n) {
  unsigned i = 0;
  std::vector<NeighbourhoodEntry> entries;
  while (i < n) {
    entries.push_back(neighbourhoods_.at(id).top());
    neighbourhoods_.at(id).pop();
    ++i;
  }

  auto ret_val = neighbourhoods_.at(id).top();

  i = 0;
  while (i < entries.size()) {
    neighbourhoods_.at(id).push(entries[i]);
    ++i;
  }

  return ret_val;
}

}  // namespace knn
