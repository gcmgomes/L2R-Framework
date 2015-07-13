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

static void StdDevFeatureValues(
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

void Normalizer::NormalizeStandardScore(::base::Query& query) {
  std::unordered_map<unsigned, double> means, std_devs;
  MeanFeatureValues(query, means);
  StdDevFeatureValues(query, means, std_devs);
  auto document = query.begin();
  while (document != query.end()) {
    auto feature = document->begin();
    while (feature != document->end()) {
      unsigned id = feature->first;
      double& val = feature->second;
      val = (val - means[id]) / std_devs[id];
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
      val = (val - mins[id]) / (maxes[id] - mins[id]);
      ++feature;
    }
    ++document;
  }
}

}  // namespace util
