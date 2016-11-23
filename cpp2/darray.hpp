#ifndef DARRAY_HPP_
#define DARRAY_HPP_

#include <cstdint>
#include <stdexcept>
#include <string>
#include <functional>

template <typename T>
class DynArray_ {
private:
  T* const data;
  uint32_t size_;
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

  // Big three
  DynArray_(uint32_t size__) : data(new T[size__]), size_(size__) { }
  DynArray_(const DynArray_& other)
	: data(new T[other.size_]), size_(other.size_) { *this = other; }
  ~DynArray_() noexcept {
	delete[] data;
  }
  DynArray_& operator=(const DynArray_& other) {
	if (size_ != other.size_) {
	  throw std::runtime_error("Cannot change array size with assignment");
	}
	for (uint32_t i = 0; i < size_; ++i) {
	  data[i] = other.data[i];
	}
	return *this;
  }
  uint32_t size() const { return size_; }
};

template <typename T>
class DynArray : public DynArray_<T> {
private:
  // Not necessarily const, use with care
  T& get(int64_t i) const { return DynArray_<T>::get(i); }
public:
  DynArray(uint32_t size_) : DynArray_<T>(size_) {}
  const T& operator[](int64_t i) const { return get(i); }
  T& operator[](int64_t i) { return get(i); }
  uint32_t size() const { return DynArray_<T>::size(); }

  // Transformations
  DynArray<T> subarray(uint32_t start, uint32_t stop) {
	if (start > stop) {
	  throw std::runtime_error("start > stop");
	}
	if (stop >= size()) {
	  throw std::runtime_error("Stop too high");
	}
	DynArray<T> result (stop - start);
	for (uint32_t i = 0; i < stop - start; ++i) {
	  result[i] = get(i + start);
	}
	return result;
  }

  template <typename U>
  DynArray<U> map(std::function<U(T, uint32_t)> f) const {
	DynArray<U> result (size());
	for (uint32_t i = 0; i < size(); ++i) {
	  result[i] = f(get(i), i);
	}
	return result;
  }
  void foreach(std::function<void(T, uint32_t)> f) const {
	for (uint32_t i = 0; i < size(); ++i) {
	  f(get(i), i);
	}
  }
  void inmap(std::function<T(T, uint32_t i)> f) {
	for (uint32_t i = 0; i < size(); ++i) {
	  get(i) = f(get(i), i);
	}
  }
};

#endif
