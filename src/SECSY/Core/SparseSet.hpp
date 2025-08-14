#pragma once

#include <cstddef>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace SECSY {

template <typename T_, typename Size_ = std::size_t>
class SparseSet {
 public:
  using size_type       = Size_;
  using value_type      = T_;
  using reference       = T_&;
  using const_reference = const T_&;
  using pointer         = T_*;
  using const_pointer   = const T_*;

  static constexpr size_type npos = std::numeric_limits<size_type>::max();

  SparseSet(size_type capacity_ = 0) {
    m_sparse.resize(capacity_, npos);
  }

  void Add(value_type e_) {
    if (Contains(e_)) {
      return;
    }

    EnsureCapacity(e_);
    m_sparse[static_cast<size_type>(e_)] = m_dense.size();
    m_dense.push_back(e_);
  }

  void Remove(value_type e_) {
    if (!Contains(e_)) {
      return;
    }

    size_type index = m_sparse[static_cast<size_type>(e_)];
    value_type last = m_dense.back();
    m_dense[index]  = last;
    m_sparse[static_cast<size_type>(last)] = index;
    m_dense.pop_back();
    m_sparse[static_cast<size_type>(e_)] = npos;
  }

  bool Contains(value_type e_) const {
    return static_cast<size_type>(e_) < m_sparse.size() &&
           m_sparse[static_cast<size_type>(e_)] != npos;
  }

  size_type Size() const {
    return m_dense.size();
  }

  const_reference operator[](size_type index_) const {
    if (index_ >= m_dense.size()) {
      throw std::out_of_range("Index out of range");
    }
    return m_dense[index_];
  }

  reference operator[](size_type index_) {
    return const_cast<reference>(std::as_const(*this).operator[](index_));
  }

  auto begin() noexcept {
    return m_dense.begin();
  }
  auto end() noexcept {
    return m_dense.end();
  }
  auto begin() const noexcept {
    return m_dense.begin();
  }
  auto end() const noexcept {
    return m_dense.end();
  }
  auto cbegin() const noexcept {
    return m_dense.cbegin();
  }
  auto cend() const noexcept {
    return m_dense.cend();
  }

 private:
  using dense_storage  = std::vector<value_type>;
  using sparse_storage = std::vector<size_type>;

  void EnsureCapacity(value_type e_) {
    if (static_cast<size_type>(e_) >= m_sparse.size()) {
      m_sparse.resize(static_cast<size_type>(e_) + 1, npos);
    }
  }

  dense_storage m_dense;
  sparse_storage m_sparse;
};

}  // namespace SECSY