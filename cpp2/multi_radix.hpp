#ifndef MULTI_RADIX_HPP_
#define MULTI_RADIX_HPP_

#include "list.hpp"
#include <stdexcept>

LinkedList<uint64_t> to_digits(uint64_t n, LinkedList<uint64_t> radix);
uint64_t from_digits(LinkedList<uint64_t> digits, LinkedList<uint64_t> radix);

LinkedList<uint64_t> to_digits(uint64_t n, LinkedList<uint64_t> radix) {
  LinkedList<uint64_t> digits;

  uint64_t place_value = 1;
  for (auto it = radix.cbegin(); it != radix.cend(); ++it) {
	place_value *= (*it);
  }

  if (n > place_value) {
	throw std::runtime_error("Radix not high enough to encode n");
  }

  for (auto it = radix.cbegin(); it != radix.cend(); ++it) {
	place_value /= (*it);
	uint64_t digit = n / place_value;
	n -= digit * place_value;
	digits.append(digit);
  }

  // assert(radix.size() == digits.size(), "size not match");
  return digits;
}

uint64_t from_digits(LinkedList<uint64_t> digits, LinkedList<uint64_t> radix) {
  assert(radix.size() == digits.size(), "size not match");

  uint64_t n = 0;
  uint64_t place_value = 1;
  radix.prepend(0); digits.prepend(0);
  {
	auto itr = radix.cend(); auto itd = digits.cend();
	--itr; --itd;
	for (; itr != radix.cbegin(); --itr, --itd) {
	  n += (*itd) * place_value;
	  place_value *= (*itr);
	}
  }
  radix.pop_front();
  return n;
}

#endif
