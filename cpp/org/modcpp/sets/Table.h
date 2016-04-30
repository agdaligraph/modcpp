#ifndef org_modcpp_sets_Table
#define org_modcpp_sets_Table

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <utility>

#include "org/modcpp/base/IteratorRange.h"

namespace org::modcpp::sets {

  using org::modcpp::base::IteratorRange;

  template<typename FieldT>
  class Table {
   public:
    typedef FieldT* Iterator;
    typedef uint32_t SizeType;
    class RowIterator;

   public:
    Table() : rows_(0), cols_(0), size_(0), data(0) {};
    Table(SizeType rows, SizeType cols);
    Table(const Table &);
    Table(Table &&);
    ~Table() { delete [] data; };

   public:
    SizeType size() const { return size_; }
    SizeType rowCount() const { return rows_; }
    SizeType colCount() const { return cols_; }
    Iterator begin() const { return data; }
    Iterator end() const { return data + size_; }

    void setZero();
    IteratorRange<RowIterator> rows() const;

    FieldT &operator()(SizeType row, SizeType col) { return data[cols_ * row + col]; }

    const FieldT& operator()(SizeType row, SizeType col) const {
      return data[cols_ * row + col];
    }

    Table operator+(const Table &rhs) const;
    Table &operator+=(const Table &rhs);

   private:
    SizeType rows_, cols_, size_;
    FieldT* data;
  };

  template<typename FieldT>
  class Table<FieldT>::RowIterator {
   public:
    RowIterator(FieldT *cellPtr, SizeType columns)
        : cellPtr(cellPtr), columns(columns), idx(0) {}

   public:
    FieldT &getCell(SizeType column) { return *(cellPtr + column); }
    SizeType index() const { return idx; }

    bool operator!=(const RowIterator &rhs) const { return cellPtr != rhs.cellPtr; }
    bool operator==(const RowIterator &rhs) const { return cellPtr == rhs.cellPtr; }
    RowIterator &operator*() { return *this; }

    RowIterator &operator++() {
      cellPtr += columns;
      ++idx;
      return *this;
    }

   private:
    FieldT *cellPtr;
    const SizeType columns;
    SizeType idx;
  };

  template<typename FieldT>
  Table<FieldT>::Table(SizeType rows, SizeType cols)
      : rows_(rows), cols_(cols), size_(rows * cols), data(new FieldT[size_]) {}

  template<typename FieldT>
  Table<FieldT>::Table(const Table<FieldT> &rhs) : rows_(rhs.rows_), cols_(rhs.cols_),
      size_(rhs.size_), data(new FieldT[size_]) {
    memcpy(data, rhs.data, size_ * sizeof(FieldT));
  }

  template<typename FieldT>
  Table<FieldT>::Table(Table<FieldT> &&rhs) : rows_(rhs.rows_), cols_(rhs.cols_),
      size_(rhs.size_), data(rhs.data) {
    rhs.data = nullptr;
  }

  template<typename FieldT>
  IteratorRange<typename Table<FieldT>::RowIterator> Table<FieldT>::rows() const {
    return IteratorRange<RowIterator>(RowIterator(data, cols_), RowIterator(data + size_, cols_));
  }

  template<typename FieldT>
  void Table<FieldT>::setZero() {
    memset(data, 0, size_ * sizeof(FieldT));
  }

  template<typename FieldT>
  Table<FieldT> Table<FieldT>::operator+(const Table<FieldT> &rhs) const {
    assert(rows_ == rhs.rows_ && "Table::operator+() row dimension does not match.");
    assert(cols_ == rhs.cols_ && "Table::operator+() column dimension does not match.");

    Table<FieldT> product(rows_, cols_);
    Iterator prodIterator = product.data;
    Iterator lhsIterator = data;
    Iterator rhsIterator = rhs.data;
    for (SizeType i = 0; i < size_; ++i) {
      *prodIterator++ = *lhsIterator++ + *rhsIterator++;
    }
    return product;
  }

  template<typename FieldT>
  Table<FieldT> &Table<FieldT>::operator+=(const Table<FieldT> &rhs) {
    assert(rows_ == rhs.rows_ && "Table::operator+=() row dimension does not match.");
    assert(cols_ == rhs.cols_ && "Table::operator+=() column dimension does not match.");

    Iterator iterator = data;
    Iterator rhsIterator = rhs.data;
    for (SizeType i = 0; i < size_; ++i) {
      *iterator++ += *rhsIterator++;
    }
    return *this;
  }

}

#endif
