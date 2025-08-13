#include <vector>
#include <stdexcept>
#include <limits>

template <typename T>
class SparseSet {
 public:
  static constexpr size_t npos = std::numeric_limits<size_t>::max();

  SparseSet(size_t capacity = 0) {
    sparse.resize(capacity, npos);
  }

  void add(T e) {
    if (contains(e)) return;
    ensureCapacity(e);
    sparse[e] = dense.size();
    dense.push_back(e);
  }

  void remove(T e) {
    if (!contains(e)) {
      throw std::runtime_error("Element not found in SparseSet");
    }
    size_t index = sparse[e];
    T last       = dense.back();
    dense[index] = last;
    sparse[last] = index;
    dense.pop_back();
    sparse[e] = npos;
  }

  bool contains(T e) const {
    return e < sparse.size() && sparse[e] != npos;
  }

  size_t size() const {
    return dense.size();
  }

  const T& operator[](size_t index) const {
    if (index >= dense.size()) {
      throw std::out_of_range("Index out of range");
    }
    return dense[index];
  }

 private:
  void ensureCapacity(T e) {
    if (e >= sparse.size()) {
      sparse.resize(e + 1, npos);
    }
  }

  std::vector<T> dense;
  std::vector<size_t> sparse;
};
