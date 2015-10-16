#include <random>
#include <chrono>
#include <ctime>
#include "smote_segmenter.h"

namespace knn {
namespace segmented_smote {

SmoteSegmenter::SmoteSegmenter(
    const ::util::Discretizer* discretizer,
    const SegmentationMetadata* segmentation_metadata,
    const ::base::Document& a, const ::base::Document& b) {
  discretizer_ = discretizer;
  segmentation_metadata_ = segmentation_metadata;
  Initialize(a, b);
}

unsigned SmoteSegmenter::Segment(::base::Document& document) {
  std::vector<double> parameters;
  SeparatingParameters(parameters);
  unsigned i = 0, best_modal_score = 2147483647, best_position = -1;
  while (i < parameters.size() - 1) {
    ::base::Document d(i);
    this->Rebuild(parameters[i], d);
    unsigned current_score = segmentation_metadata_->Score(d);
    if (best_modal_score > current_score) {
      best_position = i;
      best_modal_score = current_score;
    }
    i++;
  }

  i = best_position;
  this->GenerateSyntheticSample(parameters[i], parameters[i + 1], document);
  return best_modal_score;
}

void SmoteSegmenter::GenerateSyntheticSample(double lower, double upper,
                                             ::base::Document& document) {
  // unsigned seed = rand();
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

  std::default_random_engine generator(seed);
  std::uniform_real_distribution<double> distribution(lower, upper);

  double final_parameter = distribution(generator);
  this->Rebuild(final_parameter, document);
}

void SmoteSegmenter::Initialize(const ::base::Document& a,
                                const ::base::Document& b) {
  srand(time(NULL));
  std::unordered_set<unsigned> feature_ids;
  a.GetKnownFeatures(feature_ids);
  b.GetKnownFeatures(feature_ids);

  auto id = feature_ids.begin();
  while (id != feature_ids.end()) {
    double x_a = 0, x_b = 0;
    if (a.vector().count(*id)) {
      x_a = a.vector().at(*id);
    }
    if (b.vector().count(*id)) {
      x_b = b.vector().at(*id);
    }
    segment_edges_[*id] = std::make_pair(x_a, x_b);
    ++id;
  }
}

static double ParameterValue(double intercept,
                             std::pair<double, double>& limits) {
  if (limits.first == limits.second) {
    return -1;
  }
  return (intercept - limits.first) / (limits.second - limits.first);
}

bool SmoteSegmenter::ValidateParameter(double parameter) {
  if (parameter < 0 || 1 < parameter) {
    return false;
  }
  return true;
}

// Parameter is the |t| value of the parametric vector equation describing a
// line: (1-t) * A + t * B
void SmoteSegmenter::SeparatingParameters(std::vector<double>& parameters) {
  ::util::Discretizer::Mode mode =
      ::util::Discretizer::Mode::UNIFORM_BIN_LENGTH;
  std::unordered_set<std::string> parameter_set;
  auto feature = segment_edges_.begin();
  while (feature != segment_edges_.end()) {
    unsigned a = discretizer_->Discretize(mode, feature->second.first);
    unsigned b = discretizer_->Discretize(mode, feature->second.second);
    double limits = 1.0 / (double)discretizer_->bin_count();
    int i = a + 1;
    while (i <= b) {
      char num[128];
      double parameter = ParameterValue(limits * i, feature->second);
      sprintf(num, "%lf", parameter);
      parameter_set.insert(std::string(num));
      i++;
    }
    ++feature;
  }

  parameters.clear();
  parameter_set.insert("0");
  parameter_set.insert("1");
  auto it = parameter_set.begin();
  while (it != parameter_set.end()) {
    double parameter = 0;
    sscanf(it->c_str(), "%lf", &parameter);
    parameters.push_back(parameter);
    ++it;
  }
  std::sort(parameters.begin(), parameters.end());
}

void SmoteSegmenter::Rebuild(double parameter, ::base::Document& document) {
  double t = parameter;
  auto feature = segment_edges_.begin();
  while (feature != segment_edges_.end()) {
    double x_a = feature->second.first, x_b = feature->second.second;
    double value = (1 - t) * x_a + t * x_b;
    document.InsertDimension(feature->first, value);
    ++feature;
  }
}

}  // namespace segmented_smote

}  // namespace knn
