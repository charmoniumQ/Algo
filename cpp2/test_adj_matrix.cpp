#include <iostream>
#include "adj_matrix.hpp"

using namespace std;

int main() {

  AdjMatrix<uint32_t> a (10, false, static_cast<uint32_t>(0));
  a.insert(3, 4, 5);
  a.insert(3, 6, 2);
  a.insert(3, 7, 1);
  a.insert(1, 3, 3);
  a.insert(1, 2, 3);
  a.kruskal();

  const AdjMatrix<uint32_t> b (a);
  cout << b.neighbors(3) << endl;
  cout << b;
}
