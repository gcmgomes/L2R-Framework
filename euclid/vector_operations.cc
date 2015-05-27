#include <vector>
#include <string>
#include <cstdio>
#include <cmath>
#include "vector_operations.h"

namespace euclid {

double DotProd(const ::base::Document& d1, const ::base::Document& d2) {
  auto i = d1.vector().begin();
  double prod = 0;
  while (i != d1.vector().end()) {
    if (d2.vector().count(i->first)) {
      prod += i->second * d2.vector().at(i->first);
    }
    ++i;
  }
  return prod;
}

double EuclideanDistance(const ::base::Document& d1,
                         const ::base::Document& d2) {
  double dist = 0;

  auto i = d1.vector().begin();
  while (i != d1.vector().end()) {
    double value = 0;
    if (d2.vector().count(i->first)) {
      value = d2.vector().at(i->first);
    }
    dist += (i->second - value) * (i->second - value);
    ++i;
  }

  i = d2.vector().begin();
  while (i != d2.vector().end()) {
    if (!d1.vector().count(i->first)) {
      dist += i->second * i->second;
    }
    ++i;
  }
  return sqrt(dist);
}

double Cosine(const ::base::Document& d1, const ::base::Document& d2) {
  double cosine = DotProd(d1, d2);
  cosine /= d1.length() * d2.length();
  return cosine;
}

}  // namespace euclid
