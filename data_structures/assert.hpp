#ifndef ASSERT_HPP_
#define ASSERT_HPP_

#include <stdexcept>
#include <string>

void assert(bool, std::string);

void assert(bool thing, std::string msg) {
  if (!thing) { throw std::runtime_error(msg); }
}


#endif
