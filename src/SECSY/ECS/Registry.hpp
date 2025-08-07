#pragma once

#include <string_view>

#include "Entity.hpp"

namespace SECSY {

class Registry {
 public:
  // === Entity Management ===
  Entity Create();               // create entity
  void Destroy(Entity e);        // destroy entity
  bool IsAlive(Entity e) const;  // is entity alive

  // === Component Management ===
  template <typename T, typename... Args>
  T& Emplace(Entity e, Args&&... args);  // give entity a component

  template <typename T>
  T& Get(Entity e);  // get reference to component from entity

  template <typename T>
  bool Has(Entity e) const;  // does an entity have a component

  template <typename T>
  void Remove(Entity e);  // remove component from entity

  // === Views / Queries ===
  template <typename... Components>
  auto View();  // Returns iterable view of entities with given components

  template <typename... Include, typename... Exclude>
  auto Query();  // More advanced query with excludes
};

}  // namespace SECSY
