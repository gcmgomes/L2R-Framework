#ifndef _RI_2015_1_EUCLID_VECTOR_OPERATIONS_H_
#define _RI_2015_1_EUCLID_VECTOR_OPERATIONS_H_

#include "../base/document.h"

namespace euclid {

double DotProd(const ::base::Document& d1, const ::base::Document& d2);

double EuclideanDistance(const ::base::Document& d1,
                         const ::base::Document& d2);

double Cosine(const ::base::Document& d1, const ::base::Document& d2);

}  // namespace euclid
#endif
