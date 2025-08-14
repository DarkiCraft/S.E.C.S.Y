#pragma once

#include <cstddef>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <vector>

template <typename T_>
class SparseSet {
 public:
  static constexpr size_t npos = std::numeric_limits<size_t>::max();

  SparseSet(size_t capacity_ = 0) {
    m_sparse.resize(capacity_, npos);
  }

  void Add(T_ e_) {
    if (Contains(e_)) return;
    EnsureCapacity(e_);
    m_sparse[e_] = m_dense.size();
    m_dense.push_back(e_);
  }

  void Remove(T_ e_) {
    if (!Contains(e_)) {
      throw std::runtime_error("Element not found in SparseSet");
    }
    size_t index = m_sparse[e_];
    T_ last       = m_dense.back();
    m_dense[index] = last;
    m_sparse[last] = index;
    m_dense.pop_back();
    m_sparse[e_] = npos;
  }

  bool Contains(T_ e_) const {
    return e_ < m_sparse.size() && m_sparse[e_] != npos;
  }

  size_t Size() const {
    return m_dense.size();
  }

  const T_& operator[](size_t index) const {
    if (index >= m_dense.size()) {
      throw std::out_of_range("Index out of range");
    }
    return m_dense[index];
  }

 private:
  void EnsureCapacity(T_ e_) {
    if (e_ >= m_sparse.size()) {
      m_sparse.resize(e_ + 1, npos);
    }
  }

  std::vector<T_> m_dense;
  std::vector<size_t> m_sparse;
};
