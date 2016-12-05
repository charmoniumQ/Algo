#ifndef ADJ_MATRIX_HPP_
#define ADJ_MATRIX_HPP_

#include <iostream>
#include <algorithm>
#include "dynarray.hpp"
#include "list.hpp"
#include "dset.hpp"

template <typename Weight>
class AdjMatrix {
private:
  DynDoubleArray<Weight> data;
  bool directed;
  Weight none;

public:
  AdjMatrix(uint32_t n, bool directed_, Weight none_) :
	data(n, n),
	directed(directed_),
	none(none_) {
	// Set all edges to none
	data.inmap([none_](Weight, uint32_t, uint32_t) { return none_; });
  }

  Weight edge(uint32_t i, uint32_t j) const { return data[i][j]; }

  void insert(uint32_t i, uint32_t j, Weight w) {
	data[i][j] = w;
	if (not directed) {
	  data[j][i] = w;
	}
  }

  void remove(uint32_t i, uint32_t j) {
	if (data[i][j] != none) {
	  data[i][j] = none;
	}
	if (not directed) {
	  if (data[i][j] != none) {
		data[i][j] = none;
	  }
	}
  }

  LinkedList<uint32_t> neighbors(uint32_t i) const {
	LinkedList<uint32_t> list;
	for (uint32_t j = 0; j < size(); ++j) {
	  if (edge(i, j) != none) {
		list.append(j);
	  }
	}
	return list;
  }

  class Edge {
  public:
	Weight w;
	uint32_t i, j;
	Edge(Weight w_, uint32_t i_, uint32_t j_) {
	  this->w = w_; this->i = i_; this->j = j_;
	}
	Edge() = default;
	bool operator<(const Edge& other) const {
	  return this->w < other.w;
	}
  };

  LinkedList<Edge> edges() {
	LinkedList<Edge> list;
	for (uint32_t i = 0; i < size(); ++i) {
	  for (uint32_t j = 0; j < size(); ++j) {
		if (edge(i, j) != none) {
		  Edge e (data[i][j], i, j);
		  list.append(e);
		}
	  }
	}
	return list;
  }

  uint32_t size() const { return data.rows(); }
  bool is_directed() const { return directed; }
  Weight get_none() const { return none; }

  template <typename NewWeight>
  void map(std::function<NewWeight(Weight, uint32_t, uint32_t)> f, NewWeight none_) const {
	AdjMatrix<NewWeight> result (size(), is_directed(), none_);
  	for (uint32_t i = 0; i < size(); ++i) {
	  uint32_t j = is_directed() ? 0 : i;
  	  for (; j < size(); ++j) {
  		insert(i, j, f(data[i][j], i, j));
  	  }
  	}
  }

  void foreach(std::function<void(Weight, uint32_t, uint32_t)> f) const {
  	for (uint32_t i = 0; i < size(); ++i) {
	  uint32_t j = is_directed() ? 0 : i;
  	  for (; j < size(); ++j) {
  		f(data[i][j], i, j);
  	  }
  	}
  }

  void inmap(std::function<Weight(Weight, uint32_t, uint32_t)> f) {
  	for (uint32_t i = 0; i < size(); ++i) {
	  uint32_t j = is_directed() ? 0 : i;
  	  for (; j < size(); ++j) {
		Weight n = f(data[i][j], i, j);
  		data[i][j] = n;
		if (is_directed()) {
		  data[j][i] = n;
		}
  	  }
  	}
  }

  friend std::ostream& operator<<(std::ostream& out, const AdjMatrix<Weight>& that) {
  	that.foreach([&](Weight w, uint32_t i, uint32_t j) -> void {
		if (w != that.none) {
		  out << i << " " << j << " " << w << std::endl;
		}
  	  });
	return out;
  }

  AdjMatrix<Weight> kruskal() {
	AdjMatrix<Weight> spantree (size(), false, get_none());
	Dset<uint32_t> connectivity (size());

	LinkedList<Edge> edge_list = edges();
	Edge* edge_array = edge_list.toArray();
	std::sort(edge_array, edge_array + edge_list.size());

	for (uint32_t i = 0; i < edge_list.size(); ++i) {
	  uint32_t a = edge_array[i].i, b = edge_array[i].j;
	  if (connectivity.find(a) != connectivity.find(b)) {
		connectivity.unionf(a, b);
		spantree.insert(a, b, edge_array[i].w);
		if (connectivity.subset_size(a) == size() - 1) {
		  break;
		}
	  }
	}
	return spantree;
  }
};

#endif
