#pragma once

#include <vector>
#include <unordered_map>
#include <stdexcept>

template <typename T>
class SparseSet {
 public:
  SparseSet() = default;

  void add(const T& value) {
    if (contains(value)) return;
    sparse[value] = dense.size();
    dense.push_back(value);
  }

  void remove(const T& value) {
    if (!contains(value)) {
      throw std::runtime_error("Element not found in SparseSet");
    }
    size_t index = sparse[value];
    T last       = dense.back();
    dense[index] = last;
    sparse[last] = index;
    dense.pop_back();
    sparse.erase(value);
  }

  bool contains(const T& value) const {
    return sparse.find(value) != sparse.end();
  }

  size_t size() const noexcept {
    return dense.size();
  }

  const T& operator[](size_t index) const {
    if (index >= dense.size()) {
      throw std::out_of_range("Index out of range");
    }
    return dense[index];
  }

  auto begin() noexcept {
    return dense.begin();
  }
  auto end() noexcept {
    return dense.end();
  }
  auto begin() const noexcept {
    return dense.begin();
  }
  auto end() const noexcept {
    return dense.end();
  }
  auto cbegin() const noexcept {
    return dense.cbegin();
  }
  auto cend() const noexcept {
    return dense.cend();
  }

 private:
  std::vector<T> dense;
  std::unordered_map<T, size_t> sparse;
};
