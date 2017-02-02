#ifndef TESTER_HPP_
#define TESTER_HPP_

#include <list>
#include <string>
#include <chrono>
#include <random>
#include <cstdint>
#include <iostream>
#include <stdexcept>

template <typename Tester>
bool test(Tester& tester, uint64_t num_tests) {
  using namespace std;
  default_random_engine generator;
  uniform_int_distribution<uint8_t> branch_picker (0, tester.num_branches);
  bool success = true;
  list<string> history;
  auto startTime = chrono::high_resolution_clock::now();

  for (uint64_t i = 0; i < num_tests and success; ++i) {
  	string event;
  	try {
  	  tester.test(branch_picker(generator), event, success);
  	} catch (runtime_error e) {
  	  history.push_back(std::string("Exception [") + e.what() + std::string("] while running"));
  	}
  	history.push_back(event);
	try {
	  success = tester.check_every();
	} catch (runtime_error e) {
	  history.push_back(std::string("Exception [") + e.what() + std::string("] while checking"));
	  success = false;
	}
  }

  auto endTime = chrono::high_resolution_clock::now();
  double duration = (chrono::duration_cast<chrono::milliseconds>(endTime - startTime)).count();

  if (success) {
  	cout << "test passed" << endl << "--------------------" << endl;
	cout << duration / num_tests << "ms / iteration over " << num_tests << " iterations" << endl;
  } else {
  	cout << "test failed" << endl << "--------------------" << endl;
  	for (const string& event : history) {
  	  cout << event << endl;
  	}
	cout << tester.post_mortem() << endl;
  }
  return success;
}

#endif
