#pragma once

namespace SECSY {

template <typename Key, typename Value>
class FlatSet {
 public:
  // --- Member Types ---
  using key_type    = Key;
  using mapped_type = Value;
  using size_type   = std::size_t;

  // --- Construction / Destruction ---
  FlatSet()                              = default;
  ~FlatSet()                             = default;
  FlatSet(const FlatSet&)                = default;
  FlatSet& operator=(const FlatSet&)     = default;
  FlatSet(FlatSet&&) noexcept            = default;
  FlatSet& operator=(FlatSet&&) noexcept = default;

  // --- Capacity ---
  [[nodiscard]] bool empty() const noexcept;
  [[nodiscard]] size_type size() const noexcept;

  // --- Modifiers ---
  template <typename... Args>
  Value& Emplace(const Key& key, Args&&... args);
  bool Insert(const Key& key,
              const Value& value);  // returns false if key exists
  bool Erase(const Key& key);       // returns false if key not found
  void Clear();

  // --- Lookup ---
  [[nodiscard]] Value* Find(const Key& key) noexcept;
  [[nodiscard]] const Value* Find(const Key& key) const noexcept;
  [[nodiscard]] bool Contains(const Key& key) const noexcept;

  // --- Iteration ---
  auto begin() noexcept;
  auto end() noexcept;
  auto begin() const noexcept;
  auto end() const noexcept;

  // --- Element Access ---
  Value& operator[](const Key& key);      // inserts default if not found
  const Value& At(const Key& key) const;  // throws if not found
  Value& At(const Key& key);              // throws if not found
};

}  // namespace SECSY