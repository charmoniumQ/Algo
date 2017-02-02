#include <iostream>
#include "adj_matrix.hpp"

using namespace std;

int main() {

  AdjMatrix<int32_t> a (5, true, static_cast<uint32_t>(0));
  a.insert(0, 1, 100);
  a.insert(1, 3, -2);
  a.insert(3, 4, 2);
  a.insert(4, 2, -1);
  a.insert(2, 1, 4);
  a.insert(2, 3, 3);

  InfReal<uint32_t> i (0, true);
  InfReal<uint32_t> j = i;
  a.floyd_warshall();
}
