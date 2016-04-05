#include <cmath>
#include <sstream>
#include "bitset.h"

namespace bayes {
namespace branch_and_bound {

Bitset::Bitset() : Bitset(64){};

Bitset::Bitset(size_t bit_count) {
  bit_count_ = bit_count;
  size_t ull_count = std::max(std::ceil((double)bit_count / 64.0), 1.0);
  bits_.resize(ull_count, 0);
  bit_string_ = std::string(bit_count_, '0');
}

bool Bitset::Set(unsigned bit, bool setting) {
  unsigned ull_i = bit / 64;
  if (ull_i >= bits_.size()) {
    return false;
  }
  bit_string_[bit] = '0' + setting;
  unsigned long long mask = 1;
  mask <<= (bit % 64);
  if (setting) {
    bits_[ull_i] |= mask;
    high_bits_.insert(bit);
  } else {
    mask = ~mask;
    bits_[ull_i] &= mask;
    high_bits_.erase(bit);
  }
  return true;
}

bool Bitset::operator[](unsigned bit) {
  if (bit >= bit_count_) {
    return 0;
  }
  unsigned ull_i = bit / 64;
  bit %= 64;
  unsigned long long bytes = bits_[ull_i];
  return (bytes >> bit) & 1;
}

bool Bitset::at(unsigned bit) const {
  if (bit >= bit_count_) {
    return bits_.at(bit);
  }
  unsigned ull_i = bit / 64;
  bit %= 64;
  unsigned long long bytes = bits_.at(ull_i);
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

std::string Bitset::ToString() const {
  std::string bits = bit_string_;
  std::reverse(bits.begin(), bits.end());
  return bits;
}

Bitset Bitset::FromBitString(const std::string& bit_string) {
  Bitset bitset = Bitset(bit_string.size());
  size_t bit = 0;
  while(bit < bit_string.size()) {
    if(bit_string[bit] == '1') {
      bitset.Set(bit, true);
    }
    ++bit;
  }
  return bitset;
}

void Bitset::clear() {
  auto it = bits_.begin();
  while(it != bits_.end()) {
    *it = 0;
    ++it;
  }
  high_bits_.clear();
  bit_string_.assign(bit_count_, '0');
}

}  // namespace branch_and_bound
}  // namespace bayes
