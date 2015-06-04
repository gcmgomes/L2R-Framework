#include <random>
#include <chrono>
#include <ctime>
#include "smote.h"

namespace knn {

Smote::Smote(const ::base::Query& sample_query, unsigned relevance_level,
             unsigned k)
    : real_relevants_(sample_query.id(), sample_query.dimension_count()),
      synthetic_relevants_(sample_query.id(), sample_query.dimension_count()),
      relevance_level_(relevance_level), knn_(k) {
  Initialize(sample_query);
};

bool Smote::Initialize(const ::base::Query& query) {
  real_relevants_.mutable_documents().clear();
  synthetic_relevants_.mutable_documents().clear();

  auto doc = query.documents().begin();
  while (doc != query.documents().end()) {
    if (doc->relevance_label() == relevance_level_) {
      real_relevants_.AddDocument(doc->ToString());
    }
    ++doc;
  }

  if (real_relevants_.documents().size() < 2) {
    real_relevants_.mutable_documents().clear();
  } else {
    doc = real_relevants_.documents().begin();
    while (doc != real_relevants_.documents().end()) {
      knn_.UpdateNeighbours(real_relevants_, *doc);
      ++doc;
    }
  }

  return real_relevants_.documents().size() >= 2;
}

void Smote::Populate(unsigned extra_count) {
  srand(time(NULL));
  unsigned i = 0;
  while (i < extra_count) {
    auto doc = real_relevants_.documents().begin();
    while (doc != real_relevants_.documents().end() && i < extra_count) {
      unsigned generate = rand() % 2;
      if (generate) {
        unsigned nth_nn = rand() % knn_.neighbourhoods().at(doc->id()).size();
        unsigned nn_id = knn_.GetNthNeighbour(doc->id(), nth_nn).doc_id;
        GenerateSyntheticSample(*doc, real_relevants_.documents()[nn_id]);
        i++;
      }
      ++doc;
    }
  }
}

void Smote::GenerateSyntheticSample(const ::base::Document& d1,
                                    const ::base::Document& d2) {
  synthetic_relevants_.mutable_documents().emplace_back(
      ::base::Document(synthetic_relevants_.documents().size()));

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_real_distribution<double> distribution(0.0, 1.0);

  auto i = d1.vector().begin();
  while (i != d1.vector().end()) {
    double divergence = distribution(generator);
    double difference = std::abs(i->second - d2.vector().at(i->first));
    double base = fmin(i->second, d2.vector().at(i->first));
    synthetic_relevants_.mutable_documents().back().mutable_vector()[i->first] =
        base + divergence * difference;
    ++i;
  }
}

}  // namespce knn
