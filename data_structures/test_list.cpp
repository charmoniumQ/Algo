#include <cstdlib>
#include <cstdint>
#include <stdexcept>
#include <list>
#include <climits>
#include "tester.hpp"
#include "list.hpp"

using namespace std;

class TestList {
public:
  TestList() : random(0, USHRT_MAX) {}
  uint8_t num_branches = 3;
  void test(uint8_t branch, std::string& event, bool& success) {
	switch (branch) {
	case 0: {
	  uint16_t elem = get_random();
	  event.append("append " + to_string(elem));
	  list1.push_back(elem);
	  list2.append(elem);
	  success = true;
	  break;
	}
	case 1: {
	  uint16_t elem = get_random();
	  event.append("prepend " + to_string(elem));
	  list1.push_front(elem);
	  list2.prepend(elem);
	  success = true;
	  break;
	}
	case 2: {
	  if (!list1.empty()) {
		uint16_t elem1 = list1.back(); list1.pop_back();
		event.append("popped " + to_string(elem1) + " from back");
		uint16_t elem2 = list2.pop_back();
		success = elem1 == elem2;
	  }
	  break;
	}
	case 3: {
	  if (!list1.empty()) {
		uint16_t elem1 = list1.front(); list1.pop_front();
		event.append("popped " + to_string(elem1) + " from front");
		uint16_t elem2 = list2.pop_front();
		success = elem1 == elem2;
	  }
	  break;
	}
	default:
	  throw runtime_error("Incorrect branch chosen");
	}
  }

  bool check_every() {
	list<uint16_t>::const_iterator it1 = list1.cbegin();
	LinkedList<uint16_t>::Iterator it2 = list2.cbegin();
	for (; it1 != list1.cend() && it2 != list2.cend(); ++it1, ++it2) {
	  if (*it1 != *it2) {
		return false;
	  }
	}
	return !it2.has() && it2 == list2.cend() && it1 == list1.cend() && list1.size() == list2.size();
  }

  std::string post_mortem() {
	std::ostringstream out;
	out << "list1 = [";
	if (!list1.empty()) {
	  list<uint16_t>::const_iterator it1 = list1.cbegin();
	  out << *it1;
	  ++it1;
	  for (; it1 != list1.end(); ++it1) {
		out << ", " << *it1;
	  }
	}
	out << "]" << std::endl;
	out << "list2 = " << list2 << std::endl;
	return out.str();
  }
private:
  uint16_t get_random() {
	return random(generator);
  }
  list<uint16_t> list1;
  LinkedList<uint16_t> list2;
  default_random_engine generator;
  uniform_int_distribution<uint16_t> random;
};

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
int main(int argc, char* argv[]) {
  uint64_t num_tests = static_cast<uint64_t>(atoll(argv[1]));
  TestList t;
  bool success = test(t, num_tests);
  return success ? 0 : 1;
}
#pragma clang diagnostic pop
