#pragma once

#include <stdexcept>
#include <algorithm>
#include <utility>
#include <vector>
namespace SECSY {

template <typename Key, typename Value>
class FlatSet {
 public:
  // --- Member Types ---
  using key_type       = Key;
  using mapped_type    = Value;
  using size_type      = std::size_t;
  using value_type     = std::pair<Key, Value>;
  using container_type = std::vector<value_type>;
  using iterator       = typename container_type::iterator;
  using const_iterator = typename container_type::const_iterator;

  // --- Construction / Destruction ---
  FlatSet()                              = default;
  ~FlatSet()                             = default;
  FlatSet(const FlatSet&)                = default;
  FlatSet& operator=(const FlatSet&)     = default;
  FlatSet(FlatSet&&) noexcept            = default;
  FlatSet& operator=(FlatSet&&) noexcept = default;

  // --- Capacity ---
  [[nodiscard]] bool empty() const noexcept {
    return data.empty();
  };
  [[nodiscard]] size_type size() const noexcept {
    return data.size();
  };

  [[nodiscard]] size_type max_size() const noexcept {
    return data.max_size();
  };

  // --- Modifiers ---
  template <typename... Args>
  Value& Emplace(const Key& key, Args&&... args) {
    // find the pos for insertion
    auto it = lower_bound(data, key);
    if (it != data.end() && it->first == key) {
      // key already exist
      return it->second;
    }
    iterator inserted = data.emplace(
        it, std::make_pair(key, Value(std::forward<Args>(args)...)));

    return inserted->second;
  };
  bool Insert(const Key& key, const Value& value) {
    auto it = lower_bound(data, key);
    if (it != data.end() && it->first == key) {
      // key already exists
      return false;
    }
    data.emplace(it, std::make_pair(key, value));
    return true;
  };  // returns false if key exists
  bool Erase(const Key& key) {
    auto it = lower_bound(data, key);
    if (it == data.end() || it->first != key) {
      // key not found
      return false;
    }
    data.erase(it);
    return true;
  };  // returns false if key not found
  void Clear() {
    data.clear();
  };

  // --- Lookup ---
  [[nodiscard]] Value* Find(const Key& key) noexcept {
    auto it = lower_bound(data, key);
    if (it != data.end() && it->first == key) {
      return &it->second;
    }
    return nullptr;
  };
  [[nodiscard]] const Value* Find(const Key& key) const noexcept {
    auto it = lower_bound(data, key);
    if (it != data.end() && it->first == key) {
      return &it->second;
    }
    return nullptr;
  };
  [[nodiscard]] bool Contains(const Key& key) const noexcept {
    auto it = lower_bound(data, key);
    return it != data.end() && it->first == key;
  };

  // --- Iteration ---
  auto begin() noexcept {
    return data.begin();
  };
  auto end() noexcept {
    return data.end();
  };
  auto begin() const noexcept {
    return data.begin();
  };
  auto end() const noexcept {
    return data.end();
  };

  // --- Element Access ---
  Value& operator[](const Key& key) {
    auto it = lower_bound(data, key);
    if (it != data.end() && it->first == key) {
      return it->second;
    }
    // key not found, insert default value
    return Emplace(key);
  };  // inserts default if not found
  const Value& At(const Key& key) const {
    auto it = lower_bound(data, key);
    if (it != data.end() && it->first == key) {
      return it->second;
    }
    throw std::out_of_range("Key not found in FlatSet");
  };  // throws if not found
  Value& At(const Key& key) {
    auto it = lower_bound(data, key);
    if (it != data.end() && it->first == key) {
      return it->second;
    }
    throw std::out_of_range("Key not found in FlatSet");
  };  // throws if not found

 private:
  static iterator lower_bound(container_type& data, const Key& key) {
    return std::lower_bound(
        data.begin(),
        data.end(),
        key,
        [](const value_type& pair, const Key& k) { return pair.first < k; });
  }

  static const_iterator lower_bound(const container_type& data,
                                    const Key& key) {
    return std::lower_bound(
        data.begin(),
        data.end(),
        key,
        [](const value_type& pair, const Key& k) { return pair.first < k; });
  }

  container_type data;
};

}  // namespace SECSY
