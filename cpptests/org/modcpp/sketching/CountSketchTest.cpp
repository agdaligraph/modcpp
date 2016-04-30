#include "org/modcpp/sketching/CountSketch.h"

#include <cassert>

#include "org/modcpp/sets/Table.h"
#include "org/modcpp/string/String.h"

using org::modcpp::sets::Table;
using org::modcpp::sketching::CountSketch;
using org::modcpp::string::String;

void test_constructor() {
  CountSketch<String, int> countSketch(7, 6);
  
  assert(countSketch.rows == 128);
  assert(countSketch.cols == 64);
  assert(countSketch.columnMask == 0b00000000000000000000000000111111);
  assert(countSketch.table.rowCount() == 128);
  assert(countSketch.table.colCount() == 64);
  assert(countSketch.hashFunctions.size() == 128);
  assert(countSketch.table(1, 1) == 0);
  assert(countSketch.table(127, 63) == 0);
}

void test_get() {
  CountSketch<String, int> countSketch(7, 9); // 128 by 512 count sketch

  Table<int> &table = countSketch.table;
  for (auto row : table.rows()) {
    for (int i = 0; i < table.colCount(); ++i) {
      row.getCell(i) = 10;
    }
  }
  assert(countSketch.get("Hasan") == 10 || countSketch.get("Hasan") == -10);
  assert(countSketch.get("drama") == 10 || countSketch.get("drama") == -10);
  assert(countSketch.get("count") == 10 || countSketch.get("count") == -10);
  assert(countSketch.get("sketc") == 10 || countSketch.get("sketc") == -10);
  
  countSketch.table.setZero();
  assert(countSketch.get("Hasan") == 0);
  assert(countSketch.get("drama") == 0);
}

void test_update() {
  CountSketch<String, int> countSketch(11, 10); // 2048 by 1024 count sketch

  countSketch.update("update", 100);
  assert(countSketch.get("update") == 100);

  countSketch.update("update", -100);
  assert(countSketch.get("update") == 0);

  countSketch.update("sixty", 60);
  assert(countSketch.get("sixty") == 60);

  countSketch.update("sixty", -20);
  assert(countSketch.get("sixty") == 40);

  countSketch.update("sixty", -40);
  countSketch.update("one", 1);
  assert(countSketch.get("one") == 1);
}

void test_add() {
  CountSketch<String, int> sketch1(10, 10);
  CountSketch<String, int> sketch2 = sketch1.zeroedCopy();

  sketch1.update("coord", 10);
  sketch2.update("coord", -10);

  sketch1 += sketch2;
  assert(sketch1.get("coord") == 0);
}

int main() {
  test_constructor();
  test_get();
  test_update();
  test_add();
  return 0;
}
