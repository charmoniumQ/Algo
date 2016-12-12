#ifndef DYNARRAY_HPP_
#define DYNARRAY_HPP_

#include <cstdint>
#include <stdexcept>
#include <string>
#include <functional>

typedef uint32_t myusize;
#ifdef NEGATIVE
typedef int64_t mysize;
#else
typedef uint32_t mysize;
#endif

#ifdef NEGATIVE
myusize normalize(mysize i, myusize size);
myusize normalize /* negative version */ (int64_t i, uint32_t size) {
  if (i >= 0) {
	#ifdef MY_DEBUG
	// Throw out-of-bounds errors if I am developing/debugging this
	if (static_cast<uint32_t>(i) >= size) {
	  throw std::runtime_error(std::to_string(i) + " is out-of-bounds for size " + std::to_string(size));
	}
	#endif

	return static_cast<uint32_t>(i);
  } else {
	#ifdef MY_DEBUG
	// Throw out-of-bounds errors if I am developing/debugging this
	if (static_cast<uint32_t>(size + i) >= size or static_cast<int64_t>(size) + i < 0) {
	  throw std::runtime_error(std::to_string(i) + " is out-of-bounds for size " + std::to_string(size));
	}
	#endif

	return static_cast<uint32_t>(i + size);
  }
}
#else
inline myusize normalize(mysize i, myusize size);
inline myusize normalize /* non-negative version */ (mysize i, myusize size) {
  #ifdef MY_DEBUG
  // Throw out-of-bounds errors if I am developing/debugging this
  if (static_cast<myusize>(i) >= size) {
	throw std::runtime_error(std::to_string(i) + " is out-of-bounds for size " + std::to_string(size));
  }
  #endif
  return static_cast<myusize>(i);
}
#endif

template <typename T>
class DynArray_ {
private:
  T* data;
  myusize size_;

protected:
  T& get(mysize i) {
	return data[normalize(i, size())];
  }

  const T& get(mysize i) const {
	return data[normalize(i, size())];
  }

public:

  // Big three
  DynArray_() : DynArray_(1) { }
  DynArray_(myusize size__) : data(new T[size__]), size_(size__) { }
  DynArray_(const DynArray_& other)
	: data(new T[other.size_]), size_(other.size_) { *this = other; }
  ~DynArray_() noexcept {
	delete[] data;
  }
  DynArray_& operator=(const DynArray_& other) {
	change_size(other.size());
	for (myusize i = 0; i < size(); ++i) {
	  data[i] = other.data[i];
	}
	return *this;
  }
  void change_size(myusize size__) {
	size_ = size__;
	delete[] data;
	data = new T[size_];
  }
  myusize size() const { return size_; }
};

template <typename T>
class DynArray : public DynArray_<T> {
public:
  DynArray(myusize size_) : DynArray_<T>(size_) {}
  const T& operator[](mysize i) const { return get(i); }

  const T& get(mysize i) const { return DynArray_<T>::get(i); }
  T& get(mysize i) { return DynArray_<T>::get(i); }

  T& operator[](mysize i) { return get(i); }
  myusize size() const { return DynArray_<T>::size(); } // TODO: is this necessary?

  class Iterator {
  private:
	myusize i;
	DynArray& that;
  public:
	Iterator(myusize i_, DynArray& that_) : i(i_), that(that_) { }
	Iterator(const Iterator& other) : i(other.i), that(other.that) { }
	Iterator operator-(myusize j) { return Iterator(i - j, that); }
	Iterator operator+(myusize j) { return Iterator(i + j, that); }
	Iterator operator--(int) { Iterator other (*this); --i; return other; }
	Iterator operator++(int) { Iterator other (*this); ++i; return other; }
	Iterator& operator-=(myusize j) { i -= j; return *this; }
	Iterator& operator+=(myusize j) { i += j; return *this; }
	Iterator& operator--() { --i; return *this; }
	Iterator& operator++() { ++i; return *this; }
	Iterator operator<(const Iterator& other) const { return i < other.i; }
	Iterator operator>(const Iterator& other) const { return i > other.i; }
	Iterator operator<=(const Iterator& other) const { return i <= other.i; }
	Iterator operator>=(const Iterator& other) const { return i >= other.i; }
	Iterator operator!=(const Iterator& other) const { return i != other.i; }
	Iterator operator==(const Iterator& other) const { return i == other.i; }
	Iterator operator[](mysize j) {
	  if (j >= 0) {
		return *this + static_cast<myusize>(j);
	  }
	  #ifdef NEGATIVE
	  else {
	  	return *this - static_cast<myusize>(-j);
	  }
	  #endif
	}
	T operator*() { return that[i]; }
	T* operator->() { return *(that[i]); }
  };

  Iterator begin() { return Iterator(0, *this); }
  Iterator end() { return Iterator(size(), *this); }

  friend std::ostream& operator<<(std::ostream& os, const DynArray<T>& array) {
	os << "[";
	for (myusize i = 0; i < array.size() - 1; ++i) {
	  os << array[i] << ", ";
	}
	if (array.size() > 0) {
	  os << array[array.size()-1];
	}
	os << "]";
	return os;
  }

  // Transformations
  template <typename U>
  DynArray<U> map(std::function<U(T, myusize)> f) const {
	DynArray<U> result (size());
	for (myusize i = 0; i < size(); ++i) {
	  result[i] = f(get(i), i);
	}
	return result;
  }
  void foreach(std::function<void(T, myusize)> f) const {
	for (myusize i = 0; i < size(); ++i) {
	  f(get(i), i);
	}
  }
  void inmap(std::function<T(T, myusize i)> f) {
	for (myusize i = 0; i < size(); ++i) {
	  get(i) = f(get(i), i);
	}
  }
};

template <typename T>
class DynDoubleArray {
private:
  DynArray<T> data;
  myusize rows_, cols_;

  T& get(mysize i, mysize j) {
	return data[normalize(i, rows()) * cols() + normalize(j, cols())];
  }

  const T& get(mysize i, mysize j) const {
	return data[normalize(i, rows()) * cols() + normalize(j, cols())];
  }

  class Row {
  private:
	mysize row;
	DynDoubleArray<T>& that;
  public:
	Row(mysize row_, DynDoubleArray<T>& that_) : row(row_), that(that_) {}
	T& operator[](mysize col) { return that.get(row, col); }
  };

  class ConstRow {
  private:
  	mysize row;
  	const DynDoubleArray<T>& that;
  public:
  	ConstRow(mysize row_, const DynDoubleArray<T>& that_) : row(row_), that(that_) {}
  	const T& operator[](mysize col) const { return that.get(row, col); }

	friend std::ostream& operator<<(std::ostream& os, const ConstRow& array) {
	if (array.that.cols() == 0) {
	  os << "[]";
	} else if (array.that.cols() == 1) {
	  os << "[" << array[0] << "]";
	} else {
	  os << "[" << array[0] << ", ";
	  for (myusize i = 0; i < array.that.cols(); ++i) {
		os << array[-1] << ", ";
	  }
	  os << array[-1] << "]";
	}
	return os;
  }
  };


public:
  DynDoubleArray(myusize rows__, myusize cols__)
	: data(rows__ * cols__), rows_(rows__), cols_(cols__) { }

  myusize rows() const { return rows_; }
  myusize cols() const { return cols_; }

  Row operator[](mysize row) {
	return Row(row, *this);
  }

  ConstRow operator[](mysize row) const {
  	return ConstRow(row, *this);
  }

  template <typename U>
  DynDoubleArray<U> map(std::function<U(T, myusize, myusize)> f) const {
	DynDoubleArray<U> result (rows(), cols());
	for (myusize i = 0; i < rows(); ++i) {
	  for (myusize j = 0; j < cols(); ++j) {
		result[i][j] = f(get(i, j), i, j);
	  }
	}
	return result;
  }
  void foreach(std::function<void(T, myusize, myusize)> f) const {
	for (myusize i = 0; i < rows(); ++i) {
	  for (myusize j = 0; j < cols(); ++j) {
		f(get(i, j), i, j);
	  }
	}
  }
  void inmap(std::function<T(T, myusize, myusize)> f) {
	for (myusize i = 0; i < rows(); ++i) {
	  for (myusize j = 0; j < cols(); ++j) {
		get(i, j) = f(get(i, j), i, j);
	  }
	}
  }

  friend std::ostream& operator<<(std::ostream& os, const DynDoubleArray<T>& array) {
	if (array.rows() == 0) {
	  os << "[[]]" << std::endl;
	} else if (array.rows() == 1) {
	  os << "[" << ConstRow(0, array) << "]\n";
	} else {
	  os << "[" << ConstRow(0, array) << ",\n";
	  for (myusize r = 1; r < array.rows() - 1; ++r) {
		os << " " << ConstRow(r, array) << ",\n";
	  }
	  os << " " << ConstRow(-1, array) << "]" << std::endl;
	}
	return os;
  }

};

#endif
