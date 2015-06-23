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

void Smote::Populate(double extra_percentage, bool use_feedback) {
  unsigned i = 0;
  unsigned doc_id = 0, size = real_relevants_.documents().size();
  unsigned extra_count = ceil(size * (extra_percentage) / 100.0);
  auto doc = real_relevants_.documents().begin();
  while (i < extra_count) {
    while (!(rand() % 2)) {
      doc_id++;
      doc_id %= size;
    }
    auto doc = real_relevants_.documents().at(doc_id);
    unsigned nth_nn = rand() % knn_.neighbourhoods().at(doc.id()).size();
    unsigned nn_id = knn_.GetNthNeighbour(doc.id(), nth_nn).doc_id;
    GenerateSyntheticSample(doc, real_relevants_.documents()[nn_id]);
    std::cout << "\rQuery id: " << doc.query_id() << " Extras: " << i + 1;
    fflush(stdout);
    if (use_feedback) {
      real_relevants_.AddDocument(
          synthetic_relevants_.documents().back().ToString());
      UpdateNeighbourhoods(size, std::make_pair(doc_id, nn_id));
    }
    ++i;
  }
  std::cout << std::endl;
}

void Smote::AugmentQuery(::base::Query& query) {
  auto doc = synthetic_relevants_.begin();
  while (doc != synthetic_relevants_.end()) {
    query.AddDocument(doc->ToString());
    ++doc;
  }
}

void Smote::GenerateSyntheticSample(const ::base::Document& d1,
                                    const ::base::Document& d2) {
  synthetic_relevants_.mutable_documents().emplace_back(
      ::base::Document(synthetic_relevants_.documents().size()));

  unsigned seed = rand(); // std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_real_distribution<double> distribution(0.0, 1.0);

  auto i = d1.vector().begin();
  while (i != d1.vector().end()) {
    double divergence = distribution(generator);
    double difference = std::abs(i->second - d2.vector().at(i->first));
    double base = fmin(i->second, d2.vector().at(i->first));
    synthetic_relevants_.mutable_documents().back().mutable_vector()[i->first] =
        base + divergence * difference;

    char meta[128];
    sprintf(meta, "#synthetic = %lu, %u -- %u",
            synthetic_relevants_.mutable_documents().size(), d1.id(), d2.id());
    synthetic_relevants_.mutable_documents().back().mutable_meta_information() =
        meta;
    ++i;
  }

  synthetic_relevants_.mutable_documents().back().mutable_query_id() =
      d1.query_id();
  synthetic_relevants_.mutable_documents().back().mutable_relevance_label() =
      relevance_level_;

  /*std::cout << std::endl
            << d1.ToString() << std::endl
            << "--- " << synthetic_relevants_.documents().back().ToString()
            << std::endl
            << d2.ToString() << std::endl;*/
}

void Smote::UpdateNeighbourhoods(unsigned real_size,
                                 std::pair<unsigned, unsigned> execeptions) {
  auto synthetic = synthetic_relevants_.documents().back();
  unsigned i = 0;
  while (i < real_size) {
    auto base = real_relevants_.documents().at(i);
    knn_.UpdateNeighbour(base, synthetic);
    ++i;
  }
}

}  // namespce knn
