// ./bin < p4d0in.txt | diff p4d0out.txt -

#include <iomanip>
#include <iostream>
#include "adj_matrix.hpp"
#include "coords.hpp"

using namespace std;

int main() {
  uint16_t N; cin >> N;
  DynArray<Coord2<double> > coords (N);
  for (uint16_t i = 0; i < N; ++i) {
	cin >> coords[i].x;
	cin >> coords[i].y;
  }
  double R; cin >> R;

  AdjMatrix<double> graph (N, false, 0.0);
  for (uint16_t i = 0; i < N; ++i) {
	for (uint16_t j = 0; j < N; ++j) {
	  double dist = d(coords[i], coords[j]);
	  if (dist <= R) {
		graph.insert(i, j, dist);
	  }
	}
  }

  AdjMatrix<double> spanTree = graph.kruskal();
  spanTree.dump2();

  double total;
  spanTree.foreach([&total](double weight, uint32_t, uint32_t) {
	  total += weight;
	});
  cout << fixed << setprecision(2) << total << endl;

  AdjMatrix<uint8_t> connectivity = graph.map<uint8_t>([](double w, uint32_t, uint32_t) -> uint8_t {
	  return w != 0.0 ? 1 : 0;
	}, 0, false);

  ShortestPathTree<uint8_t> pathTree = connectivity.floyd_warshall();
  for (uint16_t i = 1; i < N; ++i) {
	cout << 1 << " ";
	LinkedList<uint32_t> path = pathTree.path(0, i);
	for (uint32_t e : path) {
	  cout << (e + 1) << " ";
	}
	cout << path.size() << endl;
  }


  return 0;
}
