#pragma once

#include <string_view>

#include "Entity.hpp"

namespace SECSY {

class Registry {
 public:
  // === Entity Management ===
  Entity CreateEntity();
  void DestroyEntity(Entity e);
  bool IsAlive(Entity e) const;

  // === Component Management ===
  template <typename T, typename... Args>
  T& AddComponent(Entity e, Args&&... args);

  template <typename T>
  T& GetComponent(Entity e);

  template <typename T>
  bool HasComponent(Entity e) const;

  template <typename T>
  void RemoveComponent(Entity e);

  // === Views / Queries ===
  template <typename... Components>
  auto View();  // Returns iterable view of entities with given components

  template <typename... Include, typename... Exclude>
  auto Query();  // More advanced query with excludes

  // === Tagging / Grouping (optional but sexy) ===
  void Tag(Entity e, std::string_view tag);
  bool HasTag(Entity e, std::string_view tag);
  std::vector<Entity> GetTagged(std::string_view tag);
};

}  // namespace SECSY
