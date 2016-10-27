#ifndef DEST_HPP_
#define DEST_HPP_

#include <iostream>
#include <iomanip>
#include "list.hpp"

template <typename int_t>
class Dset {
public:
  /* Big five */
  Dset(int_t  size__) :
	size_(size__), sets_(size_), data(new int_t[size_]), sizes(new int_t[size_]) {
	clear();
  }
  Dset(const Dset<int_t>& other) :
	size_(other.size_), sets_(other.sets_), data(new int_t[size_]), sizes(new int_t[size_]) {
	// Copy data from other
	for (int_t i = 0; i < size(); ++i) {
	  data[i] = other.data[i];
	}
  }
  ~Dset() noexcept { delete[] data; delete[] sizes; }

  /* Actual stuff */
  void unionf(int_t a, int_t b) {
	LinkedList<int_t> patha, pathb;
	find_path(a, patha);
	find_path(b, pathb);
	int_t aroot = patha.peek_back();
	int_t broot = pathb.peek_back();
	if (aroot != broot) {
	  int_t root;
	  root = (sizes[aroot] >= sizes[broot]) ? patha.pop_back() : root = pathb.pop_back();
	  sizes[root] = sizes[aroot] + sizes[broot];
	  compress(patha, root);
	  compress(pathb, root);
	  --sets_;
	}
  }
  int_t find(int_t x) {
	LinkedList<int_t> path;
	find_path(x, path);
	int_t root = path.pop_back();
	//compress(path, root);
	return root;
  }
  void clear() {
	for (int_t i = 0; i < size_; ++i) {
	  data[i] = size_;
	  sizes[i] = 1;
	}
	sets_ = size_;
  }
  int_t size() { return size_; }
  int_t sets() { return sets_; }
  void printArray() {
	using namespace std;
	for (int_t i = 0; i < size_; ++i) {
	  if (i % 20 == 0) {
		cout << endl;
	  }
	  cout << setw(4) << -static_cast<int>(i);
	}
  }
private:
  void compress(LinkedList<int_t> path, int_t root) {
	for (auto it = path.cbegin(); it != path.cend(); ++it) {
	  data[*it] = root;
	}
  }
  void find_path(int_t a, LinkedList<int_t>& list) {
	list.append(a);
	if (data[a] == size_) {
	  return;
	} else {
	  find_path(data[a], list);
	}
  }
  void print() {
	using namespace std;
	for (int_t i = 0; i < size_; ++i) {
	  cout << setw(4) << static_cast<int>(i);
	}
	cout << endl;
	for (int_t i = 0; i < size_; ++i) {
	  cout << setw(4) << static_cast<int>(data[i]);
	}
	cout << endl;
	for (int_t i = 0; i < size_; ++i) {
	  cout << setw(4) << static_cast<int>(sizes[i]);
	}
	cout << endl;
  }
  const int_t size_;
  int_t sets_;
  int_t*const data;
  int_t*const sizes;
};

#endif
