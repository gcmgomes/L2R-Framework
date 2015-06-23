#include <map>
#include <vector>
#include <string>
#include <cstdio>
#include <cmath>
#include "document.h"

namespace base {

Document::Document(unsigned id, const std::string& vector, unsigned hash_size) {
  id_ = id;
  this->vector_.reserve(hash_size);
  relevance_label_ = 0;
  if (!vector.empty()) {
    Parse(vector);
  }
}

Document::Document(unsigned id, const std::string& vector)
    : Document(id, vector, 0){};

Document::Document(unsigned id) : Document(id, ""){};

static void TokenizeVector(const std::string& vector,
                           std::vector<std::string>& tokens) {
  std::string token;
  tokens.clear();
  unsigned i = 0;
  while (i < vector.size()) {
    // IF we find a '#' we can simply read till the end of the string. This is
    // just meta information.
    if (vector.at(i) == '#') {
      while (i < vector.size()) {
        token += vector.at(i);
        i++;
      }
    }
    // If we find a space or a :, we need to split it.
    else if (vector.at(i) == ' ' || vector.at(i) == ':') {
      if (!token.empty()) {
        tokens.push_back(token);
        token.clear();
      }
    } else {
      token += vector.at(i);
    }
    i++;
  }

  if (!token.empty()) {
    tokens.push_back(token);
  }
}

void Document::Parse(const std::string& vector) {
  std::vector<std::string> tokens;
  TokenizeVector(vector, tokens);
  unsigned i = 3, reduction = 0;
  sscanf(tokens[0].c_str(), "%u", &relevance_label_);
  sscanf(tokens[2].c_str(), "%u", &query_id_);

  if (tokens.back().find("#") != std::string::npos) {
    reduction = 1;
  }

  length_ = 0;
  while (i < (tokens.size() - reduction)) {
    unsigned dimension = 0;
    double value = 0;
    sscanf(tokens[i].c_str(), "%u", &dimension);
    sscanf(tokens[++i].c_str(), "%lf", &value);
    length_ += value * value;
    InsertDimension(dimension, value);
    i++;
  }

  length_ = sqrt(length_);

  if (reduction > 0) {
    meta_information_ = tokens.back();
  } else {
    meta_information_.clear();
  }

}

// Change this to use StatusOr as soon as I know how to use templates.
double& Document::operator[](unsigned dimension) {
  return vector_[dimension];
}

void Document::InsertDimension(unsigned dimension, double value) {
  if (!vector_.count(dimension)) {
    vector_[dimension] = value;
  }
}

std::string Document::GetQueryId(const std::string& vector) {
  std::vector<std::string> tokens;
  TokenizeVector(vector, tokens);
  return tokens[2];
}

std::string Document::ToString() const {
  std::string str;
  char i[128];
  sprintf(i, "%u qid:%u", relevance_label_, query_id_);
  str += i;
  std::map<unsigned, double> sorted_vector;
  auto k = vector_.begin();
  while(k != vector_.end()) {
    sorted_vector[k->first] = k->second;
    ++k;
  }
  auto j = sorted_vector.begin();
  while(j != sorted_vector.end()) {
    sprintf(i, " %u:%lf", j->first, j->second);
    str += i;
    ++j;
  }

  if(!meta_information_.empty()) {
    str += ' ';
    str += meta_information_;
  }
  return str;
}

}  // namespace base
