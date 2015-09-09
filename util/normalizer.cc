#include "normalizer.h"

namespace util {

void Normalizer::Normalize(const Normalizer::Mode mode, ::base::Query& query) {
  switch (mode) {
    case Mode::STANDARD_SCORE:
      NormalizeStandardScore(query);
      break;
    case Mode::FEATURE_SCALING:
      NormalizeFeatureScaling(query);
      break;
  }
}

void Normalizer::NormalizeStandardScore(::base::Query& query) {
  Statistics stats(query);
  const std::unordered_map<unsigned, double> &means = stats.means(),
                                             std_devs =
                                                 stats.standard_deviations();
  auto document = query.begin();
  while (document != query.end()) {
    auto feature = document->begin();
    while (feature != document->end()) {
      unsigned id = feature->first;
      double& val = feature->second;
      if (std_devs.count(id) && std_devs.at(id) > 0.0) {
        val = (val - means.at(id)) / std_devs.at(id);
      } else {
        val = means.at(id);
      }
      ++feature;
    }
    ++document;
  }
}

static void MinMaxFeatureValues(const ::base::Query& query,
                                std::unordered_map<unsigned, double>& maxes,
                                std::unordered_map<unsigned, double>& mins) {
  auto document = query.cbegin();
  while (document != query.cend()) {
    auto feature = document->cbegin();
    while (feature != document->cend()) {
      unsigned id = feature->first;
      double val = feature->second;
      if (!maxes.count(id)) {
        maxes[id] = mins[id] = val;
      }
      maxes[id] = std::max(maxes[id], val);
      mins[id] = std::min(mins[id], val);
      ++feature;
    }
    ++document;
  }
}

void Normalizer::NormalizeFeatureScaling(::base::Query& query) {
  std::unordered_map<unsigned, double> maxes, mins;
  MinMaxFeatureValues(query, maxes, mins);
  auto document = query.begin();
  while (document != query.end()) {
    auto feature = document->begin();
    while (feature != document->end()) {
      unsigned id = feature->first;
      double& val = feature->second;
      if (maxes[id] != mins[id]) {
        val = (val - mins[id]) / (maxes[id] - mins[id]);
      } else {
        val = mins[id];
      }
      ++feature;
    }
    ++document;
  }
}

}  // namespace util
