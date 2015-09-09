#include "statistics.h"

namespace util {

static void MeanFeatureValues(const ::base::Query& query,
                              std::unordered_map<unsigned, double>& means) {
  auto document = query.cbegin();
  std::unordered_map<unsigned, double> sizes;
  while (document != query.cend()) {
    auto feature = document->cbegin();
    while (feature != document->cend()) {
      means[feature->first] += feature->second;
      sizes[feature->first] += 1.0;
      ++feature;
    }
    ++document;
  }
  auto i = means.begin();
  while (i != means.end()) {
    i->second /= sizes[i->first];
    ++i;
  }
}

static void StandardDeviationFeatureValues(
    const ::base::Query& query,
    const std::unordered_map<unsigned, double>& means,
    std::unordered_map<unsigned, double>& std_devs) {
  auto document = query.cbegin();
  std::unordered_map<unsigned, double> sizes;
  while (document != query.cend()) {
    auto feature = document->cbegin();
    while (feature != document->cend()) {
      double val = means.at(feature->first) - feature->second;
      std_devs[feature->first] += val * val;
      sizes[feature->first] += 1.0;
      ++feature;
    }
    ++document;
  }
  auto i = std_devs.begin();
  while (i != std_devs.end()) {
    i->second /= sizes[i->first] - 1.0;
    i->second = sqrt(i->second);
    ++i;
  }
}

Statistics::Statistics(const ::base::Query& query) {
  init(query);
}

void Statistics::init(const ::base::Query& query) {
  MeanFeatureValues(query, means_);
  StandardDeviationFeatureValues(query, means_, standard_deviations_);
}

double Statistics::PearsonRho(const ::base::Query& query, unsigned feature_1,
                              unsigned feature_2) {
  double rho = 0;
  auto doc = query.cbegin();
  while (doc != query.cend()) {
    if (doc->vector().count(feature_1) && doc->vector().count(feature_2)) {
      rho += doc->vector().at(feature_1) * doc->vector().at(feature_2);
    }
    ++doc;
  }
  rho -= (double)query.documents().size() * means_.at(feature_1) *
         means_.at(feature_2);
  double div = (double)(query.documents().size() - 1) *
               standard_deviations_.at(feature_1) *
               standard_deviations_.at(feature_2);

  if (div != 0.0) {
    rho /= div;
  } else {
    rho = 0;
  }

  return rho;
}

double Statistics::KendallTau(const ::base::Query& query, unsigned feature_1,
                              unsigned feature_2) {
  double tau = 0, count = 0, agrees = 0, disagrees = 0, tie_1 = 0, tie_2 = 0;
  auto doc_1 = query.cbegin();
  while (doc_1 != query.cend()) {
    auto doc_2 = doc_1;
    ++doc_2;
    while (doc_2 != query.cend()) {
      double x_1 = 0, x_2 = 0, y_1 = 0, y_2 = 0;
      if (doc_1->vector().count(feature_1)) {
        x_1 = doc_1->vector().at(feature_1);
      }
      if (doc_1->vector().count(feature_2)) {
        y_1 = doc_1->vector().at(feature_2);
      }
      if (doc_2->vector().count(feature_1)) {
        x_2 = doc_2->vector().at(feature_1);
      }
      if (doc_2->vector().count(feature_2)) {
        y_2 = doc_2->vector().at(feature_2);
      }

      tie_1 += x_1 == x_2;
      tie_2 += y_1 == y_2;

      if (x_1 > x_2 && y_1 > y_2) {
        agrees += 1.0;
      } else if (x_1 < x_2 && y_1 < y_2) {
        agrees += 1.0;
      } else if (x_1 > x_2 && y_1 < y_2) {
        disagrees += 1.0;
      } else if (x_1 < x_2 && y_1 > y_2) {
        disagrees += 1.0;
      }
      count += 1.0;
      ++doc_2;
    }

    ++doc_1;
  }

  tau = agrees - disagrees;
  tau /= sqrt((agrees + disagrees + tie_1) * (agrees + disagrees + tie_2));
  return tau;
}

}  // namespace util
