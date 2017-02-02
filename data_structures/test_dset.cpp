#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <string>
#include "tester.hpp"
#include "dset.hpp"
#include "multi_radix.hpp"

const uint8_t MAX_ELEM = 100L;
using namespace std;

class TestDset {
public:
  TestDset() : elem_dist(0, MAX_ELEM), dset(MAX_ELEM) { }
  uint8_t num_branches = MAX_ELEM;
  void test(uint8_t branch, string& event, bool& success) {
	success = true;
	if (branch < MAX_ELEM / 10 * 9) {
	  uint8_t elem1 = get_random();
	  uint8_t elem2 = get_random();
	  dset.unionf(elem1, elem2);
	  event += "Union " + to_string(elem1) + " " + to_string(elem2);
	} else {
	  dset.clear();
	  event += "Clear";
	}
  }

  bool check_every() {
	for (uint8_t i = 0; i < dset.size(); ++i) {
	  dset.find(i);
	}
	return true;
  }

  string post_mortem() { return ""; }

private:
  uint8_t get_random() { return elem_dist(generator); }
  default_random_engine generator;
  uniform_int_distribution<uint8_t> elem_dist;
  Dset<uint8_t> dset;
};

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
int main(int argc, char* argv[]) {
#pragma clang diagnostic pop
  uint8_t num_tests = static_cast<uint8_t>(atoll(argv[1]));
  TestDset t;
  bool success = test(t, num_tests);
  
  return success ? 0 : 1;
}
