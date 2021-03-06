#ifndef ARRAY_HPP_
#define ARRAY_HPP_

#include <cstdint>
#include <stdexcept>
#include <string>
#include <iostream>
#include <functional>

template <typename T, uint32_t size_>
class Array_ {
private:
  T* const data;
protected:
  // Not really const, use with care
  T& get(int64_t i) const {
	if (i >= 0) {
	  if (static_cast<uint32_t>(i) >= size_) {
		throw std::runtime_error(std::to_string(i) + " is out-of-bounds for size " + std::to_string(size_));
	  }
	  return data[i];
	} else {
	  if (static_cast<uint32_t>(size_ + i) >= size_ or static_cast<int64_t>(size_) + i < 0) {
		throw std::runtime_error(std::to_string(i) + " is out-of-bounds for size " + std::to_string(size_));
	  }
	  return data[i + size_];
	}
  }
  
public:

  // Big five
  Array_() : data(new T[size_]) { }
  Array_(const Array_& other) : data(new T[size_]) { *this = other; }
  ~Array_() noexcept {
	delete[] data;
  }
  Array_& operator=(const Array_<T, size_>& other) {
	for (uint32_t i = 0; i < size_; ++i) {
	  data[i] = other.data[i];
	}
	return *this;
  }
};

template <typename T, uint32_t size_>
class Array : public Array_<T, size_> {
private:
  // Not necessarily const, use with care
  T& get(int64_t i) const { return Array_<T, size_>::get(i); }
public:
  const T& operator[](int64_t i) const { return get(i); }
  T& operator[](int64_t i) { return get(i); }
  uint32_t size() const { return size_; }

  friend ostream& operator<<(ostream& os, const Array& dt);  

  // Transformations
  template<uint32_t start, uint32_t stop>
  Array<T, stop - start> subarray() {
	if (start > stop) {
	  throw std::runtime_error("start > stop");
	}
	if (stop >= size_) {
	  throw std::runtime_error("Stop too high");
	}
	Array<T, stop - start> result;
	for (uint32_t i = 0; i < stop - start; ++i) {
	  result[i] = get(i + start);
	}
	return result;
  }

  template <typename U>
  Array<U, size_> map(std::function<U(T, uint32_t)> f) const {
	Array<U, size_> result;
	for (uint32_t i = 0; i < size_; ++i) {
	  result[i] = f(get(i), i);
	}
	return result;
  }
  void foreach(std::function<void(T, uint32_t)> f) const {
	for (uint32_t i = 0; i < size_; ++i) {
	  f(get(i), i);
	}
  }
  void inmap(std::function<T(T, uint32_t i)> f) {
	for (uint32_t i = 0; i < size_; ++i) {
	  get(i) = f(get(i), i);
	}
  }
};

template <typename T, uint32_t sizes>
ostream& operator<<(ostream& os, const Array<T>& array) {
  os << "[";
  for (uint32_t i = 0; i < size - 1; ++i) {
	os << array[i] << ", ";
  }
  if (size > 0) {
	os << array[-1] << "]";
  }
  return os;
}

#endif
