#include <iostream>
#include <fstream>
#include "k_nearest_neighbours.h"
#include "../base/dataset.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc < 2) {
    cerr << argv[0] << " [dataset]" << endl;
    return 0;
  }
  knn::KNearestNeighbours knn(3);
  base::Dataset d;

  d.Parse(argv[1]);

  const base::Query& q = d.queries().at(0);
  auto i = q.documents().begin();
  while (i != q.documents().end()) {
    knn.UpdateNeighbours(q, *i);
    ++i;
  }
  int j = 0;
  while (j < 3) {
    auto e = knn.GetNthNeighbour(q.documents()[0].id(), j);
    cout << q.documents()[0].id() << ' ' << q.documents()[e.doc_id].ToString() << ' ' << e.distance << endl;
    ++j;
  }
}
