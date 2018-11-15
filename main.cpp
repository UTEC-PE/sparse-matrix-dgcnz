#include <iostream>

#include "matrix.h"

using namespace std;

int main(int argc, char *argv[]) {
  Matrix<int> a(5, 5);
  Matrix<int> b(5, 5);
  Matrix<int> c(5, 2);

  a.fill('r');
  b.fill('r');
  c.fill('r');
  a.print();
  b.print();
  c.print();

  (((a + b) * c) + (c * 2)).transpose().print();

  // Also overloaded operator() to return reference
  a(0, 0) = 0;

  a.print();
  return EXIT_SUCCESS;
}
