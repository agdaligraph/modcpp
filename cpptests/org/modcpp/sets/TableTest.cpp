#include "org/modcpp/algebra/Matrix.h"

#include <cassert>

using org::modcpp::algebra::Matrix;

void test_constructor() {
  Matrix<double> mat1;
  Matrix<float> mat2(100,100);
}

void test_operatorAt() {
  Matrix<int> mat(5, 5);

  mat(0, 0) = 11;
  mat(0, 1) = 12;
  mat(0, 2) = 13;
  mat(0, 3) = 14;
  mat(0, 4) = 15;

  mat(1, 0) = 1;
  mat(1, 1) = 2;
  mat(1, 2) = 3;
  mat(1, 3) = 4;
  mat(1, 4) = 5;

  mat(2, 0) = 11;
  mat(2, 1) = 12;
  mat(2, 2) = 13;
  mat(2, 3) = 14;
  mat(2, 4) = 15;

  mat(3, 0) = 1;
  mat(3, 1) = 2;
  mat(3, 2) = 3;
  mat(3, 3) = 4;
  mat(3, 4) = 5;

  mat(4, 0) = 11;
  mat(4, 1) = 12;
  mat(4, 2) = 13;
  mat(4, 3) = 14;
  mat(4, 4) = 15;

  int rowParity = 1;
  for (auto row : mat.rows()) {
    for (int i = 0; i < 5; ++i) {
      assert(row.getCell(i) == (i + 1) + 10 * rowParity);
    }
    rowParity = 1 - rowParity;
  }
}

void test_rows() {
  Matrix<int> mat(10, 10);
  
  int count = 0;
  for (auto row : mat.rows()) {
    for (int i = 0; i < 10; ++i) {
      row.getCell(i) = count++;
    }
  }

  count = 0;
  int rowIndex = 0;
  for (auto row : mat.rows()) {
    for (int i = 9; i >= 0; --i) {
      assert(row.getCell(i) == rowIndex * 10 + i);
    }
    ++rowIndex;
  }
  assert(rowIndex == 10);
}

void test_setZero() {
  Matrix<int> matrix(1000, 1000);

  for (auto row : matrix.rows()) {
    for (int i = 0; i < 1000; ++i) {
      row.getCell(i) = 1;
    }
  }

  matrix.setZero();

  for (auto row : matrix.rows()) {
    for (int i = 0; i < 1000; ++i) {
      assert(row.getCell(i) == 0);
    }
  }
}

void test_sizeRowsCols() {
  Matrix<double> mat1(101, 100);
  assert(mat1.size() == 10100);
  assert(mat1.rowCount() == 101);
  assert(mat1.colCount() == 100);

  Matrix<float> mat2(10, 1);
  assert(mat2.size() == 10);
  assert(mat2.rowCount() == 10);
  assert(mat2.colCount() == 1);
}

void test_index() {
  Matrix<double> matrix(50, 100);
  int rowIndex = 0;
  for (auto row : matrix.rows()) {
    assert(row.index() == rowIndex);
    ++rowIndex;
  }
}

void test_add() {
  Matrix<int> mat1(10, 10);
  for (auto &cell : mat1) {
    cell = 5;
  }

  Matrix<int> mat2(10, 10);
  for (auto &cell : mat2) {
    cell = 10;
  }

  Matrix<int> mat3 = mat1 + mat2;
  mat2 += mat1;
  for (auto cell : mat2) {
    assert(cell == 15);
  }

  for (auto cell : mat3) {
    assert(cell == 15);
  }
}

int main() {
  test_constructor();
  test_operatorAt();
  test_rows();
  test_setZero();
  test_sizeRowsCols();
  test_index();
  test_add();
  return 0;
}
