#pragma once

#include <cstdint>
#include <tuple>

namespace SECSY {

struct Entity {
  std::uint32_t id{};
  std::uint8_t ver{};
  // we need to pad 3 bytes here, maybe group id and ver into 32 bits

  constexpr Entity() noexcept                         = default;
  constexpr Entity(const Entity&) noexcept            = default;
  constexpr Entity& operator=(const Entity&) noexcept = default;
  constexpr Entity(std::uint32_t id_, std::uint8_t ver_) noexcept
      : id(id_), ver(ver_) {}

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

  static const Entity Null;
};

constexpr Entity Entity::Null = Entity{0, 0};

}  // namespace SECSY

namespace std {

template <>
struct hash<SECSY::Entity> {
  std::size_t operator()(const SECSY::Entity& e) const noexcept {
    // using Boost Hash Combine Algorithm
    std::size_t h = std::hash<std::uint32_t>()(e.id);
    h ^= std::hash<std::uint8_t>()(e.ver) + 0x9e3779b9 + (h << 6) + (h >> 2);
    return h;
  }
};

}  // namespace std