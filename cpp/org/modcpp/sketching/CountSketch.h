#ifndef org_modcpp_sketching_CountSketch
#define org_modcpp_sketching_CountSketch

#include <algorithm>
#include <type_traits>
#include <utility>
#include <vector>

#include "org/modcpp/hashing/KarpRabinHasher.h"
#include "org/modcpp/sets/Table.h"

namespace org::modcpp::sketching {

  using org::modcpp::sets::Table;
  using org::modcpp::hashing::KarpRabinHasher;

  template<typename DomainT, typename RangeT>
  class CountSketch {
   public:
    static_assert(std::is_signed<RangeT>::value, "CountSketch requires a signed RangeT");
    typedef uint32_t ColumnType;
    typedef uint32_t RowType;

   public:
    CountSketch(RowType logRows, ColumnType logCols);

   public:
    CountSketch zeroedCopy() const;
    void update(const DomainT coord, const RangeT value);
    RangeT get(const DomainT coord) const;
    CountSketch &operator+=(const CountSketch &rhs);

   testvisible:
    RowType rows;
    ColumnType cols;
    ColumnType columnMask;
    Table<RangeT> table;
    const std::vector<KarpRabinHasher> hashFunctions;
  };

  template<typename DomainT, typename RangeT>
  CountSketch<DomainT, RangeT>::CountSketch(RowType logRows, ColumnType logCols)
      : rows(1 << logRows), cols(1 << logCols), columnMask((1 << logCols) - 1),
        table(rows, cols), hashFunctions(rows) {
    assert(logRows <= (8 * sizeof(RowType)) - 1 && "Log rows too high");
    assert(logCols <= (8 * sizeof(ColumnType)) - 1 && "Log cols too high");
    table.setZero();
  }

  // TODO(saglam): avoid copying the table
  template<typename DomainT, typename RangeT>
  CountSketch<DomainT, RangeT> CountSketch<DomainT, RangeT>::zeroedCopy() const {
    CountSketch<DomainT, RangeT> copy = *this;
    copy.table.setZero();
    return copy;
  }

  template<typename DomainT, typename RangeT>
  RangeT CountSketch<DomainT, RangeT>::get(DomainT coord) const {
    RangeT values[rows];
    for (auto row : table.rows()) {
      ColumnType hashValue = hashFunctions[row.index()].hash(coord);
      ColumnType column = (hashValue >> 1) & columnMask;
      RangeT cellValue = row.getCell(column);
      values[row.index()] = (hashValue & 1) ? cellValue : -cellValue;
    }
    std::nth_element(values, values + (rows >> 1), values + rows);
    return values[rows >> 1];
  }

  template<typename DomainT, typename RangeT>
  void CountSketch<DomainT, RangeT>::update(DomainT coord, RangeT value) {
    for (auto row : table.rows()) {
      ColumnType hashValue = hashFunctions[row.index()].hash(coord);
      ColumnType column = (hashValue >> 1) & columnMask;
      row.getCell(column) += (hashValue & 1) ? value : -value;
    }
  }

  template<typename DomainT, typename RangeT>
  CountSketch<DomainT, RangeT> &CountSketch<DomainT, RangeT>::operator+=(
      const CountSketch<DomainT, RangeT> &rhs) {
    table += rhs.table;
    return *this;
  }

} // namespace

#endif
