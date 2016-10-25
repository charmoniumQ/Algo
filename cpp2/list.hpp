#ifndef LIST_HPP_
#define LIST_HPP_

#include <stdexcept>
#include <sstream>

void assert(bool, std::string);

typedef std::runtime_error ex;

void assert(bool thing, std::string msg) {
  if (!thing) { throw ex(msg); }
}

template <typename T>
class LinkedList {
private:
  class Node {
  public:
	Node(T data_) : data(data_), prev(nullptr), next(nullptr) { }
	void static link(Node* left, Node* right) {
	  using namespace std;
	  assert(left == nullptr || right == nullptr || left != right, "Cannot link the same two elements");
	  assert(left == nullptr || left->valid(), "Cannot link; left is not valid");
	  assert(right == nullptr || right->valid(), "Cannot link; right is not valid");
	  if (left != nullptr) { left->next = right; }
	  if (right != nullptr) { right->prev = left; }
	  assert(left == nullptr || left->valid(), "left is not valid");
	  assert(right == nullptr || right->valid(), "right is not valid");
	}
	bool valid() {
	  //checks that the double-link property is preserved
	  assert(prev == nullptr || prev->next == this, "prev is not linked to this");
	  assert(next == nullptr || next->prev == this, "next is not linked to this");
	  assert(prev != this, "prev is this");
	  assert(next != this, "next is this");
	  assert(prev == nullptr || next == nullptr || prev != next, "triangular link");
	  return true;
	}

	Node* get_next() { return next; }
	Node* get_prev() { return prev; }
	T& get_data() { return data; }

  private:
	T data;
	Node* prev; Node* next;
  };
  Node* head = nullptr; Node* tail = nullptr;
  uint64_t size_ = 0;

public:
  // Big five
  LinkedList() {
	tail = head = nullptr;
	size_ = 0;
  }
  LinkedList(const LinkedList<T>& other) {
	for (auto it = other.cbegin(); it.has(); ++it) {
	  append(*it);
	}
  }
  LinkedList(LinkedList<T>&& other) noexcept {
	tail = other.tail;
	head = other.head;
	other.head = other.tail = nullptr;
  }
  ~LinkedList() {
	while (!empty()) { pop_front(); }
  }
  LinkedList<T>& operator=(const LinkedList<T>& other) {
	while (!empty()) { pop_front(); }
	for (auto it = other.cbegin(); other.has(); +it) {
	  append(*it);
	}
	return *this;
  }
  LinkedList<T>& operator=(const LinkedList<T>&& other) {
	while (!empty()) { pop_front(); }
	tail = other.tail;
	head = other.head;
	other.head = other.tail = nullptr;
	return *this;
  }

  // Modify
  void append(T data) {
	if (empty()) {
	  tail = head = new Node(data);
	} else {
	  Node* oldtail = tail;
	  tail = new Node(data);
	  Node::link(oldtail, tail);
	}
	++size_;
  }
  void prepend(T data) {
	if (empty()) {
	  tail = head = new Node(data);
	} else {
	  Node* oldhead = head;
	  head = new Node(data);
	  Node::link(head, oldhead);
	}
	++size_;
  }

  // Peek
  T& peek_front() {
	if (not empty()) {
	  return head->get_data();
	} else { throw ex("Can't peek from empty list"); }
  }
  T& peek_back() {
	if (not empty()) {
	  return tail->get_data();
	} else { throw ex("Can't peek from empty list"); }
  }

  // Delete
  T pop_front() {
	if (!empty()) {
	  valid();
	  --size_;
	  T data = head->get_data();
	  Node* oldhead = head;
	  if (empty()) {
		head = tail = nullptr;
	  } else {
		head = head->get_next();
		Node::link(nullptr, head);
	  }
	  delete oldhead;
	  valid();
	  return data;
	} else {
	  throw ex("Can't pop from empty list");
	}
  }
  T pop_back() {
	if (!empty()) {
	  valid();
	  --size_;
	  T data = tail->get_data();
	  Node* oldtail = tail;
	  if (empty()) {
		head = tail = nullptr;
	  } else {
		tail = tail->get_prev();
		Node::link(tail, nullptr);
	  }
	  delete oldtail;
	  valid();
	  return data;
	} else {
	  throw ex("Can't pop from empty list");
	}
  }
  T* raw_array() {
	if (not empty()) {
	  T* out = new T[size()];
	  uint64_t ctr = 0;
	  for (Iterator it = cbegin(); it != cend(); ++it, ++ctr) {
		out[ctr] = *it;
	  }
	  return out; // You are in charge of deleteing this
	} else { return nullptr; }
  }
  T** raw_ptr_array() {
	if (not empty()) {
	  T** out = new T*[size()];
	  uint64_t ctr = 0;
	  for (Iterator it = cbegin(); it != cend(); ++it, ++ctr) {
		out[ctr] = &*it;
	  }
	  return out; // You are in charge of deleteing this
	} else { return nullptr; }
  }

  // Size info
  bool empty() const { return size_ == 0; }
  uint64_t size() const { return size_; }

  // Traversal
  class Iterator {
  public:
	Iterator(Node* prev_, Node* current_) : prev(prev_), current(current_) {}
	bool has() const { return current != nullptr; }
	bool operator==(const Iterator& other) { return current == other.current; }
	bool operator!=(const Iterator& other) { return ! (*this == other); }
	const T& operator*() {
	  if (has()) { return current->get_data(); }
	  else { throw ex("Can't dereference invalid iterator"); }
	}
	T* operator->() {
	  if (has()) { return &current->get_data(); }
	  else { throw ex("Can't dereference invalid iterator"); }
	}
	Iterator& operator++() {
	  if (has()) { prev = current; current = current->get_next(); }
	  else { throw ex("Can't increment past the end"); }
	  return *this;
	}
	Iterator& operator--() {
	  current = prev; prev = prev->get_prev();
	  return *this;
	}
	Iterator operator++(int) {
	  Iterator other (*this);
	  ++*this;
	  return other;
	}
	Iterator operator--(int) {
	  Iterator other (*this);
	  --*this;
	  return other;
	}
  private:
	Node* prev = nullptr;
	Node* current = nullptr;
  };

  Iterator cbegin() const { return Iterator(nullptr, head); }
  Iterator cend() const { return Iterator(tail, nullptr); }
  std::ostream& dump(std::ostream& out) const {
	if (empty()) {
	  out << "[]";
	} else {
	  Iterator it = cbegin();
	  out << "[" << std::to_string(*it);
	  ++it;
	  for (; it != cend(); ++it) {
		out << ", " << std::to_string(*it);
	  }
	  out << "]";
	}
	return out;
  }

  bool valid() const {
	// Checks that size, head, and tail are correct and checks that each node is valid
	if (empty()) {
	  assert(size_ == 0, "size != 0 on empty list");
	  assert(head == nullptr, "head not null on empty list");
	  assert(tail == nullptr, "tail not null on empty list");
	} else {
	  uint64_t size_test = 1;
	  Node* c = head;
	  assert(head != nullptr, "head is null on non-empty list");
	  while (c->get_next() != nullptr) {
		assert(c->valid(), "not valid");
		++size_test;
		c = c->get_next();
	  }
	  assert(size_ == size_test, "size is " + std::to_string(size_) + " should be " + std::to_string(size_test));
	  assert(c == tail, "tail is incorrect");
	}
	return true;
  }
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const LinkedList<T>& list) { return list.dump(out); }

#endif
