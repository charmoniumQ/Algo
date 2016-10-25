#ifndef TRIE_HPP_
#define TRIE_HPP_

#include <functional>
#include <stdexcept>
#include <sstream>
#include "list.hpp"
#include "pair.hpp"


// #include <string>
// #include <iostream>
// #include <string.h>

#ifndef LIST_HPP_
void assert(bool, std::string);

typedef std::runtime_error ex;

void assert(bool thing, std::string msg) {
  if (not thing) { throw ex(msg); }
}
#endif

template <typename T>
class Trie {
private:
  class Node {
  public:
	Node(uint8_t branches_) : degree(branches_) {
	  children = new Node*[degree];
	  for (uint8_t i = 0; i < degree; ++i) {
		children[i] = nullptr;
	  }
	  terminal = false;
	}
	~Node() {
	  for (uint8_t i = 0; i < degree; ++i) {
		delete children[i];
	  }
	  delete[] children;
	}

	uint8_t live_degree() {
	  uint8_t ctr = 0;
	  for (uint8_t i = 0; i < degree; ++i) {
		if (children[i] != nullptr) {
		  ++ctr;
		}
	  }
	  return ctr;
	}
	std::string str(const Trie<T>& trie) {
	  switch (live_degree()) {
	  case 0:
		return "";
	  case 1: {
		std::ostringstream out;
		for (uint8_t i = 0; i < degree; ++i) {
		  if (children[i] != nullptr) {
			out << trie.from_int(i) << children[i]->str(trie);
			return out.str();
		  }
		}
		return "never happens";
	  }
	  default: {
		std::ostringstream out;
		out << "[";
		bool first = false;
		for (uint8_t i = 0; i < degree; ++i) {
		  if (children[i] != nullptr) {
			if (! first) {
			  first = true;
			  out << trie.from_int(i) << children[i]->str(trie);
			} else {
			  out << " " << trie.from_int(i) << children[i]->str(trie);
			}
		  }
		}
		out << "]";
		return out.str();
	  }
	  }
	}

	static Node* find(const T* s, uint64_t size, const Trie<T>& trie, Node* root) {
	  if (size == 0) {
		return root;
	  } else {
		uint8_t pos = trie.to_int(*s);
		assert(pos < root->degree, "Converter is giving " + std::to_string(pos) + " which is out of bounds for a trie of degree " + std::to_string(root->degree));
		Node* dst = root->children[pos];
		if (dst == nullptr) {
		  return nullptr;
		}
		return find(s + 1, size - 1, trie, dst);
	  }
	}
	static Node* findm(const T* s, uint64_t size, const Trie<T>& trie, Node* root) {
	  if (size == 0) {
		return root;
	  } else {
		uint8_t pos = trie.to_int(*s);
		assert(pos < root->degree, "Converter is giving " + std::to_string(pos) + " which is out of bounds for a trie of degree " + std::to_string(root->degree));
		Node*& dst = root->children[pos];
		if (dst == nullptr) {
		  // TODO: dst = 
		  root->children[pos] = new Node(trie.branches);
		}
		return findm(s + 1, size - 1, trie, dst);
	  }
	}
	bool terminal;
	uint8_t degree;
	Node** children;
	std::string id;
  };
  Node* root;
  uint8_t branches;
  std::function<uint8_t(T)> to_int;
  std::function<T(uint8_t)> from_int;
  uint64_t size_;

public:
  // Construct
  Trie(uint8_t branches_, std::function<uint8_t(T)> to_int_, std::function<T(uint8_t)> from_int_) :
	root(new Node(branches_)),
	branches(branches_),
	to_int(to_int_),
	from_int(from_int_),
	size_(0) { }

  // Modify
  bool insert(const T* s, uint64_t size) {
	Node* node = Node::findm(s, size, *this, root);
	node->id = std::string(s);
	bool present = node->terminal;
	node->terminal = true;
	if (not present) { ++size_; }
	return not present;
  }
  bool is_present(const T* s, uint64_t size) {
	Node* node = Node::find(s, size, *this, root);
	bool present = node != nullptr and node->terminal;
	return present;
  }
  bool remove(const T* s, uint64_t size) {
	// TODO: delete things
	Node* node = Node::find(s, size, *this, root);
	bool present = node != nullptr and node->terminal;
	if (present) {
	  --size_;
	  node->terminal = false;
	}
	// if (node->live_degree() == 0) {
	//   delete node;
	// }
	return present;
  }

  // Traversal
  class Iterator {
  public:
	Iterator(const Trie* trie_) :
	  trie(trie_), current(trie_->root), start_from(0), visited(), thread() {
	  if (not current->terminal) {
		next();
	  }
	  	  suppress = false;
	}
	bool has_next() {
	  return current != nullptr;
	}
	LinkedList<T> get() {
	  if (has_next()) {
		return thread;
	  } else { throw std::runtime_error("Does not have next element"); }
	}
	void next() {
	  next_();
	  while (current != nullptr and (suppress or not current->terminal)) {
		next_();
	  }
	}

  private:
	void next_() {
	  if (not dive_down()) { // if not able to dive down any further
		if (not bubble_up()) { // if not able to bubble up either
		  current = nullptr;
		  return;
		}
	  }
	}
	bool dive_down() {
	  for (uint8_t i = start_from; i < trie->branches; ++i) {
		if (current->children[i] != nullptr) {
		  Pair<const Node*, uint8_t> next (current, i + 1);
		  visited.append(next);
		  thread.append(trie->from_int(i));
		  current = current->children[i];
		  start_from = 0;
		  suppress = false;
		  return true;
		}
	  }
	  return false;
	}
	bool bubble_up() {
	  if (not visited.empty()) {
		thread.pop_back();
		Pair<const Node*, uint8_t> temp = visited.pop_back();
		current = temp.a();
		start_from = temp.b();
		suppress = true;
		return true;
	  } else {
		return false;
	  }
	}
	bool suppress;
	const Trie* trie;
	const Node* current;
	uint8_t start_from;
	LinkedList<Pair<const Trie::Node*, uint8_t> > visited;
	LinkedList<T> thread;
  };

  Iterator begin() {
	Iterator f (this);
	return f;
  }

  // Info
  uint64_t size() { return size_; }
  std::string str() {
  	std::ostringstream out;
  	out << "{";
  	for (Iterator it = begin(); it.has_next(); it.next()) {
  	  LinkedList<T> elem = it.get();
  	  out << elem << ", ";
  	}
  	out.seekp(-2, std::ios_base::end);
  	out << "}";
  	return out.str();
  	// return root->str(*this);
  }
};

#endif
