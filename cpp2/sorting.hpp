#ifndef SORTING_HPP_
#define SORTING_HPP_

template <typename Array, typename T>
void swap(Array& array, uint32_t a, uint32_t b) {
  T tmp = array[a];
  array[a] = array[b];
  array[b] = tmp;
}

template <typename ArraySrc, typename ArrayDest>
void copy(ArraySrc& src, uint32_t src_begin, ArrayDest& dest, uint32_t dest_begin, uint32_t n) {
  for (uint32_t i = 0; i < n; ++i) {
	dest[dest_begin + i] = src[src_begin + i];
  }
}

template <typename ArraySrc, typename ArrayDest>
void merge(ArrayDest& dest, uint32_t dest_begin, ArraySrc& src1, uint32_t start1, uint32_t end1, ArraySrc& src2, uint32_t start2, uint32_t end2) {
  uint32_t i = start1;
  uint32_t j = start2;
  uint32_t p = dest_begin;
  while (i < end1 and j < end2) {
	if (src1[i] < src2[j]) {
	  dest[p++] = src1[i++];
	} else {
	  dest[p++] = src2[j++];
	}
  }
  for (; i < end1; ++i, ++p) { dest[p] = src1[i]; }
  for (; j < end2; ++j, ++p) { dest[p] = src2[j]; }
}

template <typename Array, typename T>
void mergeSort(Array& array, uint32_t begin, uint32_t end) {
  uint32_t n = end - begin;
  if (n <= 1) {
  } else if (n == 2) {
	if (array[begin] > array[begin + 1]) {
	  swap<Array, T>(array, begin, begin + 1);
	}
  } else if (n == 3) {
	if (array[begin] > array[begin + 1]) {
	  swap<Array, T>(array, begin, begin + 1);
	}
	if (array[begin + 1] > array[begin + 2]) {
	  swap<Array, T>(array, begin + 1, begin + 2);
	}
	if (array[begin] > array[begin + 1]) {
	  swap<Array, T>(array, begin, begin + 1);
	}
  } else {
	// odd  n -> n = 2k+1, [0, k) and [k, 2k+1), sizes k and k+1
	// even n -> n = 2k, [0, k) and [k, 2k), sizes k and k
	uint32_t k = n / 2;
	mergeSort<Array, T>(array, begin, begin + k);
	mergeSort<Array, T>(array, begin + k, end);
	T* tmpArray = new T[n];

	merge(tmpArray, 0, array, begin, begin + k, array, begin + k, end);

	copy<T*, Array>(tmpArray, 0, array, begin, n);
  }
}
// Uncle Paul
// Andrea Frescas American Children's Cancer Society
#endif
