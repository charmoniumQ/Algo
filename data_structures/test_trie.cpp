#include <cstdlib>
#include <cstdint>
#include <set>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include "tester.hpp"
#include "trie.hpp"

using namespace std;

uint8_t alpha_to_int(char);
char int_to_alpha(uint8_t);
uint8_t alpha_to_int(char c) { return c == '\0' ? 26 : static_cast<uint8_t>(c - 'a'); }
char int_to_alpha(uint8_t i) { return i == 26 ? '\0' : static_cast<char>(i + 'a'); }

class TestTrie {
public:
  TestTrie() : mset(), trie(27, alpha_to_int, int_to_alpha), random_size(0, 10), random_char(0, 26 - 1) {}
  uint8_t num_branches = 1;
  void test(uint8_t branch, string& event, bool& success) {
	success = true;
	switch (branch) {
	case 0: {
	  string elem = random_string();
	  event.append("Insert " + elem);
	  bool ret1 = mset.insert(elem).second;
	  bool ret2 = trie.insert(elem.c_str(), elem.size() + 1);
	  success = ret1 == ret2;
	  if (ret1 != ret2) {
		throw runtime_error("Trie records element '" + elem + "' insterted" + std::to_string(ret1) + " but set records as " + std::to_string(ret2));
	  }
	  break;
	}
	case 1:
	  if (!mset.empty()) {
		vector<string> vector (mset.begin(), mset.end());
		uniform_int_distribution<uint64_t> random_picker (0, vector.size() - 1);
		uint64_t pick = random_picker(generator);
		string elem = vector[pick];
		event.append("Remove " + elem);
		set<string>::iterator it = mset.find(elem); mset.erase(it);
		bool ret2 = trie.remove(elem.c_str(), elem.size() + 1);
		if (!ret2) {
		  throw runtime_error("Trie does not record '" + elem + "' as present on remove");
		}
		break;
	  } else {
		event.append("Nothing");
	  }
	  break;
	default:
	  throw runtime_error("Incorrect branch chosen");
	}
  }

  bool check_every() {
	for (Trie<char>::Iterator it = trie.begin(); it.has_next(); it.next()) {
	  char* c = it.get().raw_array();
	  if (c != nullptr) {
		std::string s (c);
		bool found = mset.find(s) != mset.end();
		if (not found) {
		  throw runtime_error(s + " found in trie but not in set");
		}
	  }
	  delete[] c;
	}
	for (set<string>::iterator it = mset.cbegin(); it != mset.cend(); ++it) {
	  if (not trie.is_present(it->c_str(), it->size() + 1)) {
		throw runtime_error(*it + " found in set but not in trie");
	  }
	}
	return true;
  }

  string post_mortem() {
	ostringstream out;
	out << "trie = " << trie.str() << "\nmset = [";
	for (auto it = mset.cbegin(); it != mset.cend(); ++it) {
	  out << *it << " ";
	}
	out << "]";
	return out.str();
  }

private:
  string random_string() {
	string out (random_size(generator), ' ');
	for (uint8_t i = 0; i < out.size(); ++i) {
	  out[i] = int_to_alpha(random_char(generator));
	}
	return out;
  }
  set<string> mset;
  Trie<char> trie;
  default_random_engine generator;
  uniform_int_distribution<uint8_t> random_size;
  uniform_int_distribution<uint8_t> random_char;
};

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
int main(int argc, char* argv[]) {
#pragma clang diagnostic pop
  uint64_t num_tests = static_cast<uint64_t>(atoll(argv[1]));
  TestTrie t;
  bool success = test(t, num_tests);
  return success ? 0 : 1;
}
