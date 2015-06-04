#ifndef _RI_2015_1_KNN_K_NEAREST_NEIGBBOURS_H_
#define _RI_2015_1_KNN_K_NEAREST_NEIGBBOURS_H_

#include <cstdlib>
#include <iostream>
#include <queue>
#include <unordered_map>
#include "../base/document.h"
#include "../base/query.h"

namespace knn {

struct NeighbourhoodEntry {
  NeighbourhoodEntry(){};

  NeighbourhoodEntry(unsigned doc_id_, double distance_)
      : doc_id(doc_id_), distance(distance_){};

  bool operator()(const NeighbourhoodEntry& e1,
                  const NeighbourhoodEntry& e2) const {
    return e1.distance < e2.distance;
  }

  unsigned doc_id;
  double distance;
};

// This way, the top of the heap holds the element to be eliminated.
typedef std::priority_queue<NeighbourhoodEntry, std::vector<NeighbourhoodEntry>,
                            NeighbourhoodEntry> NearestNeighbourhood;

class KNearestNeighbours {
 public:
  KNearestNeighbours(unsigned k) : k_(k){};

  unsigned k() const {
    return k_;
  }

  const std::unordered_map<unsigned, NearestNeighbourhood>& neighbourhoods()
      const {
    return neighbourhoods_;
  }

  std::unordered_map<unsigned, NearestNeighbourhood>& mutable_neighbourhoods() {
    return neighbourhoods_;
  }

  // Updates the neighbourhood information for |base_document|.
  void UpdateNeighbour(const ::base::Document& base_document,
                       const ::base::Document& updating_document);

  // Updates the underlying heaps to include only the |k_| closest elements.
  void UpdateNeighbours(const ::base::Query& query,
                        const ::base::Document& document);

  // Returns the n-th neighbour of document |id|. To get the 1st element, n = 0.
  NeighbourhoodEntry GetNthNeighbour(unsigned id, unsigned n);

 private:
  // The amount of neighbours stored by each heap.
  unsigned k_;

  // The mapping document_id -> nearest neighbour heap.
  std::unordered_map<unsigned, NearestNeighbourhood> neighbourhoods_;
};

}  // namespace knn

#endif
