#include "bitset.h"
#include <algorithm>
#include <cmath>
#include <sstream>

namespace bayes {
namespace branch_and_bound {

Bitset::Bitset() : Bitset(64){};

Bitset::Bitset(size_t bit_count) {
  bit_count_ = bit_count;
  size_t ull_count = std::max(std::ceil((double)bit_count / 8.0), 1.0);
  bits_.resize(ull_count, 0);
  bits_.shrink_to_fit();
}

bool Bitset::Set(unsigned bit, bool setting) {
  unsigned ull_i = bit / 8;
  if (ull_i >= bits_.size()) {
    return false;
  }
  unsigned char mask = 1;
  mask <<= (bit % 8);
  if (setting) {
    bits_[ull_i] |= mask;
  } else {
    mask = ~mask;
    bits_[ull_i] &= mask;
  }
  return true;
}

bool Bitset::operator[](unsigned bit) {
  return this->at(bit);
}

bool Bitset::at(unsigned bit) const {
  if (bit >= bit_count_) {
    return 0;
  }
  unsigned ull_i = bit / 8;
  bit %= 8;
  unsigned char bytes = bits_.at(ull_i);
  return (bytes >> bit) & 1;
}

bool Bitset::operator==(const Bitset& bitset) const {
  unsigned i = 0;
  while (i < bits_.size()) {
    if (bitset.bits()[i] != this->bits()[i]) {
      return false;
    }
    i++;
  }
  return true;
}

static std::string Stringfy(unsigned char bits, size_t count) {
  std::string str;
  unsigned i = 0;
  while (i < count && i < 8) {
    str += '0' + (bits & 1);
    bits >>= 1;
    i++;
  }
  return str;
}

const std::vector<unsigned> Bitset::high_bits() const {
  unsigned bit = 0;
  unsigned ull = 0;
  std::vector<unsigned> high_bits;
  while (ull < bits_.size() && bit < bit_count_) {
    unsigned char b = bits_.at(ull);
    while (b) {
      if (b & 1) {
        high_bits.push_back(bit);
      }
      bit++;
      b >>= 1;
    }
    ull++;
    bit = ull * 8;
  }
  return high_bits;
}

bool Bitset::BitwiseLogicalAnd(const Bitset& bitset) const {
  unsigned ull = 0;
  while (ull < bitset.bits().size() && ull < bits_.size()) {
    unsigned char t_ull = bits_.at(ull), o_ull = bitset.bits().at(ull);
    if (t_ull & o_ull) {
      return true;
    }
    ull++;
  }
  return false;
}

bool Bitset::Contains(const Bitset& bitset) const {
  auto super_high_bits = high_bits();
  auto sub_high_bits = bitset.high_bits();
  auto it = sub_high_bits.cbegin();
  while (it != sub_high_bits.cend()) {
    if (!std::binary_search(super_high_bits.cbegin(), super_high_bits.cend(),
                            *it)) {
      return false;
    }
    ++it;
  }
  return true;
}

const std::string Bitset::bit_string() const {
  unsigned ull = 0;
  std::string bit_str;
  while (ull < bits_.size()) {
    bit_str += Stringfy(bits_.at(ull), bit_count_ - (ull * 8));
    ++ull;
  }
  return bit_str;
}

std::string Bitset::ToString() const {
  std::string bits = bit_string();
  std::reverse(bits.begin(), bits.end());
  return bits;
}

Bitset Bitset::FromBitString(const std::string& bit_string) {
  Bitset bitset = Bitset(bit_string.size());
  size_t bit = 0;
  while (bit < bit_string.size()) {
    if (bit_string[bit] == '1') {
      bitset.Set(bit, true);
    }
    ++bit;
  }
  return bitset;
}

size_t Bitset::Hash() const {
  unsigned ull = 0;
  size_t hash = 0;
  while (ull < bits_.size()) {
    if (!ull) {
      hash = std::hash<unsigned char>()(bits_.at(ull));
    } else {
      size_t rhs = std::hash<unsigned char>()(bits_.at(ull));
      hash ^= rhs + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
    ull++;
  }
  return hash;
}

void Bitset::clear() {
  auto it = bits_.begin();
  while (it != bits_.end()) {
    *it = 0;
    ++it;
  }
}

}  // namespace branch_and_bound
}  // namespace bayes
