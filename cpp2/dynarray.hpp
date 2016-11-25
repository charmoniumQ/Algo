#ifndef DYNARRAY_HPP_
#define DYNARRAY_HPP_

#include <cstdint>
#include <stdexcept>
#include <string>
#include <functional>

uint32_t normalize(int64_t i, uint32_t size);
uint32_t normalize(int64_t i, uint32_t size) {
  if (i >= 0) {
	if (static_cast<uint32_t>(i) >= size) {
	  throw std::runtime_error(std::to_string(i) + " is out-of-bounds for size " + std::to_string(size));
	}
	return static_cast<uint32_t>(i);
	} else {
	if (static_cast<uint32_t>(size + i) >= size or static_cast<int64_t>(size) + i < 0) {
	  throw std::runtime_error(std::to_string(i) + " is out-of-bounds for size " + std::to_string(size));
	}
	return static_cast<uint32_t>(i + size);
	}
}

template <typename T>
class DynArray_ {
private:
  T* data;
  uint32_t size_;

protected:
  // Not really const, use with care
  T& get(int64_t i) const {
	return data[normalize(i, size())];
  }
  
public:

  // Big three
  DynArray_() : DynArray_(1) { }
  DynArray_(uint32_t size__) : data(new T[size__]), size_(size__) { }
  DynArray_(const DynArray_& other)
	: data(new T[other.size_]), size_(other.size_) { *this = other; }
  ~DynArray_() noexcept {
	delete[] data;
  }
  DynArray_& operator=(const DynArray_& other) {
	change_size(other.size());
	for (uint32_t i = 0; i < size(); ++i) {
	  data[i] = other.data[i];
	}
	return *this;
  }
  void change_size(uint32_t size__) {
	size_ = size__;
	delete[] data;
	data = new T[size_];
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
  uint32_t size() const { return DynArray_<T>::size(); } // TODO: is this necessary?

  friend std::ostream& operator<<(std::ostream& os, const DynArray<T>& array) {
  os << "[";
  for (uint32_t i = 0; i < array.size() - 1; ++i) {
	os << array[i] << ", ";
  }
  if (array.size() > 0) {
	os << array[-1];
  }
  os << "]";
  return os;
}

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

template <typename T>
class DynDoubleArray {
private:
  DynArray<T> data;
  uint32_t rows_, cols_;

  T& get(uint64_t i, uint64_t j) const {
	return data[normalize(i, rows()) * cols() + normalize(j, cols())];
  }

  class Row {
  private:
	uint32_t row;
	const DynDoubleArray<T>& that;
  public:
	Row(uint32_t row_, const DynDoubleArray<T>& that_) : row(row_), that(that_) {}
	T& operator[](uint64_t col) { return that.get(row, col); }
  };

public:
  DynDoubleArray(uint32_t rows__, uint32_t cols__)
	: data(rows__ * cols__), rows_(rows__), cols_(cols__) { }

  uint32_t rows() const { return rows_; }
  uint32_t cols() const { return cols_; }

  Row operator[](uint64_t row) {
	return Row(row, *this);
  }

  template <typename U>
  DynArray<U> map(std::function<U(T, uint32_t, uint32_t)> f) const {
	DynArray<U> result (data.size());
	for (uint32_t i = 0; i < rows(); ++i) {
	  for (uint32_t j = 0; j < cols(); ++j) {
		result[i] = f(get(i, j), i, j);
	  }
	}
	return result;
  }
  void foreach(std::function<void(T, uint32_t, uint32_t)> f) const {
	for (uint32_t i = 0; i < rows(); ++i) {
	  for (uint32_t j = 0; j < cols(); ++j) {
		f(get(i, j), i, j);
	  }
	}
  }
  void inmap(std::function<T(T, uint32_t i)> f) {
	for (uint32_t i = 0; i < rows(); ++i) {
	  for (uint32_t j = 0; j < cols(); ++j) {
		get(i, j) = f(get(i, j), i, j);
	  }
	}
  }

};

#endif
