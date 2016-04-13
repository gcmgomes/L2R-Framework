// A class containing a more lightweight bitset.
//
// Attentio:All bits are 0 indexed!
//
#ifndef _RI_2015_1_BAYES_BRANCH_AND_BOUND_BITSET_H_
#define _RI_2015_1_BAYES_BRANCH_AND_BOUND_BITSET_H_

#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <functional>

namespace bayes {
namespace branch_and_bound {

class Bitset {
 public:
  Bitset();
  Bitset(size_t bit_count);

  // |bit| determines the position to be modified. |setting| determines the
  // value of the indexed position. Returns |true| on success, false otherwise.
  bool Set(unsigned bit, bool setting);

  // If access is out of bounds, returns 0 on default.
  bool operator[](unsigned bit);

  bool at(unsigned bit) const;

  bool operator==(const Bitset& bitset) const;

  size_t bit_count() const {
    return bit_count_;
  }

  const std::vector<unsigned long long>& bits() const {
    return bits_;
  }

  const std::vector<unsigned> high_bits() const;

  // Both bitsets must have the same size for this operation to make sense!
  // Returns true if there is at least one bit set on both sets, false
  // otherwise.
  bool BitwiseLogicalAnd(const Bitset& bitset) const;

  // This is the raw bit string, the most significant bit has the highest
  // position.
  const std::string bit_string() const;

  // Reverse of bit_string.
  std::string ToString() const;

  static Bitset FromBitString(const std::string& bit_string);

  size_t Hash() const;

  void clear();

 private:
  // Number of bits in the bitset.
  size_t bit_count_;

  // Container for tbe wanted bits.
  std::vector<unsigned long long> bits_;
  // std::set<unsigned> high_bits_;
  // std::string bit_string_;
};

}  // namespce branch_and_bound
}  // namespace bayes

namespace std {
template <>
struct hash<bayes::branch_and_bound::Bitset> {
  size_t operator()(const bayes::branch_and_bound::Bitset& bitset) const {
    return bitset.Hash();
  }
};
}  // namespace std

#endif
