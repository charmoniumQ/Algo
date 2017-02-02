#include <iostream>
#include "array.hpp"
#include "dynarray.hpp"

using namespace std;

int main() {
  {
	Array<int, 8> t, u;
	t.inmap([](int, uint32_t i) {return i * 2;});
	u = t;
	Array<float, 8> g;
	g = u.map<float>([](int x, uint32_t) -> float {return static_cast<float>(x);});
	const Array<float, 3> b (g.subarray<2, 5>());
	cout << "2 4 8" << endl;
	b.foreach([](float x, uint64_t) { cout << x << " ";});
	cout << endl;
	const float d = b[2];
	cout << d << endl;
	cout << 8 << endl;
  }

  {
	DArray<int> t (8); DArray<int> u (8);
	t.inmap([](int, uint32_t i) {return i * 2;});
	u = t;
	DArray<float> g (8);
	g = u.map<float>([](int x, uint32_t) -> float {return static_cast<float>(x);});
	const DArray<float> b (g.subarray(2, 5));
	cout << "2 4 8" << endl;
	b.foreach([](float x, uint64_t) { cout << x << " ";});
	cout << endl;
	const float d = b[2];
	cout << d << endl;
	cout << 8 << endl;
  }

  return 0;
}
