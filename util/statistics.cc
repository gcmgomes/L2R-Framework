#include <unordered_set>
#include <cmath>
#include <algorithm>
#include "statistics.h"

namespace util {

// Calculates the Mean Average Precision at K.
// Receives a vector of pairs <Classification, Expectation>.
long double Statistics::Mapk(std::vector<std::pair<double, unsigned> >
                        v_ranker, unsigned k)
{
  if(v_ranker.size()==0 || k==0) return 0;
  
  sort(v_ranker.begin(), v_ranker.end());
  reverse(v_ranker.begin(), v_ranker.end());
  
  long double average_precision=0;
  long double mean_average_precision=0;
  unsigned precision=0, i=0;

  for(i = 0; i < std::min((unsigned)v_ranker.size(), k); i++)
  {
    if(v_ranker[i].first != 0 && v_ranker[i].second != 0)
      precision++;
    average_precision = ((double)precision)/(i+1);
    mean_average_precision+=average_precision;
  }
  return mean_average_precision/i;
}

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
  double n = query.documents().size(), total = (n * (n - 1)) / 2;
  double divider = sqrt((total - tie_1) * (total - tie_2));
  if (divider == 0) {
    return 0;
  }
  tau /= divider;
  return tau;
}

static std::pair<double, double> make_pair(const ::base::Document& d,
                                           unsigned f_1_, unsigned f_2_) {
  double x_1 = 0, y_1 = 0;
  if (d.vector().count(f_1_)) {
    x_1 = d.vector().at(f_1_);
  }
  if (d.vector().count(f_2_)) {
    y_1 = d.vector().at(f_2_);
  }
  return std::make_pair(x_1, y_1);
}

double count_ties(std::unordered_map<std::string, double>& vals) {
  auto tie = vals.begin();
  double total = 0;
  while (tie != vals.end()) {
    total += (tie->second * (tie->second - 1)) / 2;
    ++tie;
  }
  return total;
}

void extract_pairs(const ::base::Query& query, unsigned f_1, unsigned f_2,
                   std::vector<std::pair<double, double> >& pairs,
                   double& ties_1, double& ties_2, double& ties_3) {
  int i = 0;
  auto doc = query.cbegin();
  pairs.resize(query.documents().size());
  std::unordered_map<std::string, double> vals_1, vals_2, vals_3;
  while (doc != query.cend()) {
    pairs[i] = make_pair(*doc, f_1, f_2);

    char v1[128], v2[128];
    sprintf(v1, "%lf", pairs[i].first);
    sprintf(v2, "%lf", pairs[i].second);

    vals_1[v1]++;
    vals_2[v2]++;

    std::string str = v1;
    str += v2;
    vals_3[str]++;

    i++;
    ++doc;
  }

  ties_1 = count_ties(vals_1);
  ties_2 = count_ties(vals_2);
  ties_3 = count_ties(vals_3);
}

bool comparer(const std::pair<double, double>& a,
              const std::pair<double, double>& b) {
  if (a.first == b.first) {
    return a.second < b.second;
  } else {
    return a.first < b.first;
  }
}

bool inner_comparer(const std::pair<double, double>& a,
                    const std::pair<double, double>& b) {
  return a.second < b.second;
}

std::string str_seq(std::vector<std::pair<double, double> >& points, unsigned l,
                    unsigned u) {
  auto i = points.begin() + l;
  std::string ret_val = "{";
  while (i != points.begin() + u) {
    char num[128];
    sprintf(num, " %lf,", i->second);
    ret_val += num;
    i++;
  }
  ret_val += "}";
  return ret_val;
}
unsigned Merge(std::vector<std::pair<double, double> >& points, unsigned l,
               unsigned m, unsigned u) {
  unsigned n = m - l, swaps = 0;
  unsigned i = l, j = m;
  unsigned t = 1;
  while (i < m && j < u) {
    if (points[j].second < points[i].second) {
      swaps += n - t + 1;
      j++;
    } else {
      t++;
      i++;
    }
  }
  return swaps;
}

unsigned Swaps(std::vector<std::pair<double, double> >& pairs, unsigned l,
               unsigned u) {
  using namespace std;
  if (l == u - 1 || l == u) {
    return 0;
  }
  unsigned m = (l + u) / 2;
  unsigned left = Swaps(pairs, l, m);
  unsigned right = Swaps(pairs, m, u);
  sort(pairs.begin() + l, pairs.begin() + m, inner_comparer);
  sort(pairs.begin() + m, pairs.begin() + u, inner_comparer);
  unsigned M = Merge(pairs, l, m, u);
  return left + right + M;
}

double Statistics::FastKendallTau(const ::base::Query& query,
                                  unsigned feature_1, unsigned feature_2) {
  double ties_1 = 0, ties_2 = 0, ties_3 = 0;
  std::vector<std::pair<double, double> > pairs;
  extract_pairs(query, feature_1, feature_2, pairs, ties_1, ties_2, ties_3);
  std::sort(pairs.begin(), pairs.end(), comparer);
  unsigned S = Swaps(pairs, 0, pairs.size()),
           total = (pairs.size() * (pairs.size() - 1)) / 2;
  double numerator = total - ties_1 - ties_2 + ties_3 - 2 * S;
  double divider = sqrt((total - ties_1) * (total - ties_2));
  if (divider == 0) {
    return 0;
  }
  return numerator / divider;
}

}  // namespace util
