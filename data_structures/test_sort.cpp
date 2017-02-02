#include <iostream>
#include"dynarray.hpp"
#include "sorting.hpp"
using namespace std;

int main() {
  DynArray<int> t (13);
  t[0] = 10;
  t[1] = 20;
  t[2] = -20;
  t[3] = 3;
  t[4] = 32;
  t[5] = 30;
  t[6] = 43;
  t[7] = 23;
  t[8] = -30;
  t[9] = 49;
  t[10] = 34;
  t[11] = 35;
  t[12] = -3;

  mergeSort<DynArray<int>, int>(t, 0, 13);
  cout << t << endl;

  return 0;
}
