#pragma once

#include <cstdint>
#include <tuple>

namespace SECSY {

struct Entity {
  using id_type  = std::uint32_t;
  using ver_type = std::uint8_t;

  id_type id{};
  ver_type ver{};
  // we need to pad 3 bytes here, maybe group id and ver into 32 bits

  constexpr Entity() noexcept                         = default;
  constexpr Entity(const Entity&) noexcept            = default;
  constexpr Entity& operator=(const Entity&) noexcept = default;
  constexpr Entity(id_type id_, ver_type ver_) noexcept : id(id_), ver(ver_) {}

  constexpr bool IsValid() const noexcept {
    return id != 0 && ver != 0;
  }

  constexpr bool operator==(const Entity& other_) const noexcept {
    return id == other_.id && ver == other_.ver;
  }

  constexpr bool operator!=(const Entity& other_) const noexcept {
    return !(*this == other_);
  }

  constexpr bool operator<(const Entity& other_) const noexcept {
    return std::tie(id, ver) < std::tie(other_.id, other_.ver);
  }

  constexpr bool operator>(const Entity& other_) const noexcept {
    return std::tie(id, ver) > std::tie(other_.id, other_.ver);
  }

  static const Entity Null;
};

constexpr Entity Entity::Null = Entity{0, 0};

}  // namespace SECSY

namespace std {

template <>
struct hash<SECSY::Entity> {
  std::size_t operator()(const SECSY::Entity& e) const noexcept {
    // using Boost Hash Combine Algorithm
    std::size_t h = std::hash<SECSY::Entity::id_type>()(e.id);
    h ^= std::hash<SECSY::Entity::ver_type>()(e.ver) + 0x9e3779b9 + (h << 6) +
         (h >> 2);
    return h;
  }
};

}  // namespace std