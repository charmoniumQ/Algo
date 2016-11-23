#include <iostream>
#include "array.hpp"

using namespace std;

int main() {
  Array<int, 8> t, u;
  t.inmap([](int, uint32_t i) {return i * 2;});
  u = t;
  Array<float, 8> g;
  // T = int
  // U = float
  // f = int -> float
  g = u.map<float>([](int x, uint32_t) -> float {return static_cast<float>(x);});
  const Array<float, 3> b (g.subarray<2, 5>());
  b.foreach([](float x, uint64_t) { cout << x << endl;});
  const float d = b[2];
  cout << d << endl;
  return 0;
}
