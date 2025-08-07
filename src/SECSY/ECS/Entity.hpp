#pragma once

#include <cstdint>
#include <functional>

namespace SECSY {

struct Entity {
  std::uint32_t id;
  std::uint8_t ver;
  // we need to pad 3 bytes here, maybe group id and ver into 32 bits

  Entity() = default;
  Entity(std::uint32_t id_, std::uint8_t ver_) : id(id_), ver(ver_) {}

  bool IsValid() const {
    return id != 0 && ver != 0;
  }

  bool operator==(const Entity& other_) const {
    return id == other_.id && ver == other_.ver;
  }

  bool operator!=(const Entity& other_) const {
    return !(*this == other_);
  }
};

}  // namespace SECSY

namespace std {

template <>
struct hash<SECSY::Entity> {
  std::size_t operator()(const SECSY::Entity& e) const {
    // using Boost Hash Combine Algorithm
    std::size_t h = std::hash<std::uint32_t>()(e.id);
    h ^= std::hash<std::uint8_t>()(e.ver) + 0x9e3779b9 + (h << 6) + (h >> 2);
    return h;
  }
};

}  // namespace std