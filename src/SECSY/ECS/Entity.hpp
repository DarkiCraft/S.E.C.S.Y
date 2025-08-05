#pragma once

#include <cstdint>
#include <functional>

namespace SECSY {

struct Entity {
  std::uint32_t id;
  std::uint32_t generation;

  bool operator==(const Entity& other_) const {
    return id == other_.id && generation == other_.generation;
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
    return (std::hash<uint32_t>()(e.id) << 1) ^
           std::hash<uint32_t>()(e.generation);
  }
};
}  // namespace std