#ifndef PAIR_TPP_
#define PAIR_TPP_

#include <sstream>

template <typename A, typename B>
class Pair {
public:
  Pair(A a_, B b_) : da(a_), db(b_) { }
  A& a() { return da; }
  B& b() { return db; }
  std::ostream& dump(std::ostream& out) const {
	return out << "(" << da << ", " << db << ")";
  }
private:
  A da; B db;
};

template <typename A, typename B>
std::ostream& operator<<(std::ostream& out, const Pair<A, B>& pair) { return pair.dump(out); }

#endif
