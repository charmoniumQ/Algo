#ifndef ADJ_MATRIX_HPP_
#define ADJ_MATRIX_HPP_

#include <iostream>
#include <iomanip>
#include <algorithm>
#include "dynarray.hpp"
#include "list.hpp"
#include "dset.hpp"
#include "infreal.hpp"

template <typename Weight> class ShortestPathTree;

template <typename Weight>
class AdjMatrix {
private:
  DynDoubleArray<Weight> data;
  bool directed__;
  Weight none;

public:
  AdjMatrix(uint32_t n, bool directed, Weight none_) :
	data(n, n),
	directed__(directed),
	none(none_) {
	// Set all edges to none
	data.inmap([none_](Weight, uint32_t, uint32_t) { return none_; });
  }

  Weight edge(uint32_t i, uint32_t j) const { return data[i][j]; }

  void insert(uint32_t i, uint32_t j, Weight w) {
	data[i][j] = w;
	if (not is_directed()) {
	  data[j][i] = w;
	}
  }

  void remove(uint32_t i, uint32_t j) {
	if (data[i][j] != none) {
	  data[i][j] = none;
	}
	if (not is_directed()) {
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
	uint32_t i, j;
	Weight w;
	Edge(uint32_t i_, uint32_t j_, Weight w_) {
	  this->i = i_; this->j = j_; this->w = w_;
	}
	Edge() = default;
	bool operator<(const Edge& other) const {
	  return this->w < other.w;
	}
	friend std::ostream& operator<<(std::ostream& out, const Edge& e) {
	  return out << e.i << " <=(" << e.w << ")=> " << e.j;
	}
  };

  LinkedList<Edge> edges() {
	using namespace std;
	LinkedList<Edge> list;
	for (uint32_t i = 0; i < size(); ++i) {
	  for (uint32_t j = 0; j < size(); ++j) {
		if (edge(i, j) != none) {
		  Edge e (i, j, data[i][j]);
		  list.append(e);
		}
	  }
	}
	return list;
  }

  uint32_t size() const { return data.rows(); }
  bool is_directed() const { return directed__; }
  Weight get_none() const { return none; }

  template <typename NewWeight>
  AdjMatrix<NewWeight> map(std::function<NewWeight(Weight, uint32_t, uint32_t)> f, NewWeight none_, bool directed = false, bool all = false) const {
	AdjMatrix<NewWeight> result (size(), is_directed(), none_);
  	for (uint32_t i = 0; i < size(); ++i) {
	  uint32_t j = is_directed() || directed ? 0 : i;
  	  for (; j < size(); ++j) {
		if (all || data[i][j] != none) {
		  result.insert(i, j, f(data[i][j], i, j));
		}
  	  }
  	}
	return result;
  }

  void foreach(std::function<void(Weight, uint32_t, uint32_t)> f, bool directed = false, bool all = false) const {
  	for (uint32_t i = 0; i < size(); ++i) {
	  uint32_t j = is_directed() || directed ? 0 : i;
  	  for (; j < size(); ++j) {
		if (all || data[i][j] != none) {
		  f(data[i][j], i, j);
		}
  	  }
  	}
  }

  void inmap(std::function<Weight(Weight, uint32_t, uint32_t)> f, bool directed = false, bool all = false) {
  	for (uint32_t i = 0; i < size(); ++i) {
	  uint32_t j = is_directed() || directed ? 0 : i;
  	  for (; j < size(); ++j) {
		if (all || data[i][j] != none) {
		  Weight n = f(data[i][j], i, j);
		  data[i][j] = n;
		  if (is_directed() || directed) {
			data[j][i] = n;
		  }
		}
  	  }
  	}
  }

  void dump(std::ostream& out, bool directed = false, bool all = false) const {
  	foreach([&](Weight w, uint32_t i, uint32_t j) -> void {
		out << i << " " << j << " " << w << std::endl;
  	  }, directed, all);
  }

  friend std::ostream& operator<<(std::ostream& out, const AdjMatrix<Weight>& that) {
	that.dump(out, false, false);
	return out;
  }

  void dump2() {
	foreach([&](Weight w, uint32_t i, uint32_t j) -> void {
		if (w != none) {
		  std::cout << (i + 1) << " " << (j + 1) << " " << std::fixed << std::setprecision(2) << w << std::endl;
		}
  	  });
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
		if (connectivity.subset_size(a) == size()) {
		  break;
		}
	  }
	}

	delete[] edge_array;
	return spantree;
  }

  ShortestPathTree<Weight> floyd_warshall() const {
	ShortestPathTree<Weight> p (size(), *this);
	return p;
  }
};

template <typename Weight>
class ShortestPathTree {
private:
  AdjMatrix<InfReal<Weight> > lengths;
  AdjMatrix<uint32_t> paths;
  uint32_t size;
public:
  ShortestPathTree(uint32_t size_, const AdjMatrix<Weight>& graph)
	: lengths (size_, true, InfReal<Weight>(0, true)),
	  paths (size_, true, 90),
	  size(size_) {
	graph.foreach([&](Weight w, uint32_t i, uint32_t j) -> void {
		lengths.insert(i, j, InfReal<Weight>(w));
		paths.insert(i, j, j);
	  }, true, false);
	// for (uint32_t i = 0; i < size; ++i) {lengths.insert(i, i, 0);}
	for (uint32_t k = 0; k < size; ++k) {
	  for (uint32_t i = 0; i < size; ++i) {
		for (uint32_t j = 0; j < size; ++j) {
		  if (i != k and i != j and k != j)
		  {
			relax(i, k, j);
		  }
		}
	  }
	}
  }

  void relax(uint32_t a, uint32_t b, uint32_t c) {
	// Is the current a -> ... -> c slower than a -> ... -> b -> ... -> c
	if (lengths.edge(a, c) > lengths.edge(a, b) + lengths.edge(b, c)) {
	  //std::cout << a << "<--->" << c << " worse than " << a << "<-" << b << "->" << c << ": " << lengths.edge(a, c) << ">" << lengths.edge(a, b) << "+" << lengths.edge(b, c) << std::endl;
	  lengths.insert(a, c, lengths.edge(a, b) + lengths.edge(b, c));
	  paths.insert(a, c, paths.edge(a, b));
	} else {
	  //std::cout << a << "<--->" << c << " better than " << a << "<-" << b << "->" << c << ": " << lengths.edge(a, c) << "<=" << lengths.edge(a, b) << "+" << lengths.edge(b, c) << std::endl;
	}
  }

  LinkedList<uint32_t> path(uint32_t a, uint32_t b) {
	LinkedList<uint32_t> path_;
	while (a != b) {
	  a = paths.edge(a, b);
	  path_.append(a);
	}
	return path_;
  }
};

#endif
