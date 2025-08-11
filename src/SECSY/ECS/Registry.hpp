#pragma once

#include <cstddef>
#include <flat_map>
#include <memory>
#include <queue>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <unordered_set>

#include "Entity.hpp"

namespace Internal {

using ComponentID =
    std::uintptr_t;  // holds address of static variable as unique ID

// TypeID implementation
template <typename T_>
ComponentID TypeID() noexcept {
  static int dummy;  // every T_ will get a unique dummy static var
  return reinterpret_cast<ComponentID>(&dummy);  // address ensures unique id
}

// Base class for all storages
struct IComponentStorage {
  virtual ~IComponentStorage()                   = default;
  virtual ComponentID TypeID() const noexcept    = 0;
  virtual void Remove(SECSY::Entity e_) noexcept = 0;
};

template <typename T_>
class ComponentStorage : public IComponentStorage {
 public:
  ComponentID TypeID() const noexcept override {
    return Internal::TypeID<T_>();
  }

  template <typename... Args_>
  T_& Emplace(SECSY::Entity e_, Args_&&... args_) {
    auto [it, inserted] = m_data.try_emplace(e_, std::forward<Args_>(args_)...);
    if (!inserted) {
      if constexpr (std::is_nothrow_constructible_v<T_, Args_...> ||
                    !std::is_nothrow_move_constructible_v<T_>) {
        std::destroy_at(std::addressof(it->second));
        std::construct_at(std::addressof(it->second),
                          std::forward<Args_>(args_)...);
      } else {
        T_ tmp(std::forward<Args_>(args_)...);  // may throw; strong guarantee
        std::destroy_at(std::addressof(it->second));
        std::construct_at(std::addressof(it->second), std::move(tmp));
      }
    }
    return it->second;
  }

  T_& Get(SECSY::Entity e_) {
    auto it = m_data.find(e_);
    if (it == m_data.end()) {
      throw std::out_of_range("component not found for entity");
    }
    return it->second;
  }

  const T_& Get(SECSY::Entity e_) const {
    auto it = m_data.find(e_);
    if (it == m_data.end()) {
      throw std::out_of_range("component not found for entity");
    }
    return it->second;
  }

  bool Has(SECSY::Entity e_) const noexcept {
    return m_data.find(e_) != m_data.end();
  }

  void Remove(SECSY::Entity e_) noexcept {
    m_data.erase(e_);
  }

 private:
  std::flat_map<SECSY::Entity, T_> m_data;
};

}  // namespace Internal

namespace SECSY {
class Registry {
 public:
  Entity Create() {
    uint32_t id;
    uint8_t ver;

    if (m_free_entities.empty()) {
      id  = m_next_id++;
      ver = 1;
    } else {
      Entity e = m_free_entities.front();
      m_free_entities.pop();
      id  = e.id;
      ver = (e.ver == 255) ? 1 : e.ver + 1;  // wrap to 1 on overflow, skip 0
    }

    Entity e{id, ver};
    m_entities.insert(e);
    return e;
  }

  void Destroy(Entity e_) {
    // Remove all components of entity
    auto it = m_entity_to_component_ids.find(e_);
    if (it != m_entity_to_component_ids.end()) {
      for (auto comp_id : it->second) {
        auto storage_it = m_storages.find(comp_id);
        if (storage_it != m_storages.end()) {
          storage_it->second->Remove(e_);
        }
      }
      m_entity_to_component_ids.erase(it);
    }

    // Remove entity from live entities
    auto removed = m_entities.erase(e_);
    if (removed) {
      m_free_entities.push(e_);
    }
  }

  bool IsAlive(Entity e_) const {
    auto it = m_entities.find(e_);
    return it != m_entities.end();
  }

  template <typename T_, typename... Args_>
  T_& Emplace(Entity e_, Args_&&... args_) {
    if (!IsAlive(e_)) {
      throw std::out_of_range("Emplace() on non-alive entity");
    }

    auto* storage = EnsureStorage<T_>();
    auto& comp    = storage->Emplace(e_, std::forward<Args_>(args_)...);

    auto comp_id = Internal::TypeID<T_>();
    m_entity_to_component_ids[e_].insert(comp_id);

    return comp;
  }

  template <typename T_>
  T_& Get(Entity e) {
    if (!IsAlive(e)) {
      throw std::out_of_range("entity is not alive");
    }

    auto* storage = FindStorage<T_>();
    if (!storage) {
      throw std::out_of_range("component storage missing");
    }
    return storage->Get(e);
  }

  template <typename T_>
  const T_& Get(Entity e) const {
    if (!IsAlive(e)) {
      throw std::out_of_range("entity is not alive");
    }

    auto* storage = FindStorage<T_>();
    if (!storage) {
      throw std::out_of_range("component storage missing");
    }
    return storage->Get(e);
  }

  template <typename T_>
  bool Has(Entity e_) const noexcept {
    if (!IsAlive(e_)) {
      return false;
    }

    auto id = Internal::TypeID<T_>();
    auto it = m_storages.find(id);

    if (it == m_storages.end()) {
      return false;  // cannot have T_ component if no Storage exists
    }

    return static_cast<const Internal::ComponentStorage<T_>*>(it->second.get())
        ->Has(e_);
  }

  template <typename T_>
  void Remove(Entity e_) noexcept {
    if (!IsAlive(e_)) {
      return;
    }

    if (auto* storage = FindStorage<T_>()) {
      storage->Remove(e_);
    }

    auto comp_id = Internal::TypeID<T_>();
    auto it      = m_entity_to_component_ids.find(e_);
    if (it != m_entity_to_component_ids.end()) {
      auto& comp_ids = it->second;
      comp_ids.erase(comp_id);

      if (comp_ids.empty()) {
        m_entity_to_component_ids.erase(it);
      }
    }
  }

  // // === Views / Queries ===
  template <typename... Components>
  auto View() {
    using ViewTuple = std::tuple<Entity, Components&...>;
    std::vector<ViewTuple> result;

    for (auto entity : m_entities) {
      if ((Has<Components>(entity) && ...)) {
        result.emplace_back(entity, Get<Components>(entity)...);
      }
    }

    return result;
  }

 private:
  std::unordered_set<Entity> m_entities;
  std::queue<Entity> m_free_entities;
  std::uint32_t m_next_id{1};
  std::unordered_map<Internal::ComponentID,
                     std::unique_ptr<Internal::IComponentStorage>>
      m_storages;
  std::unordered_map<Entity, std::unordered_set<Internal::ComponentID>>
      m_entity_to_component_ids;

  template <typename T_>
  Internal::ComponentStorage<T_>* FindStorage() noexcept {
    auto id = Internal::TypeID<T_>();
    auto it = m_storages.find(id);
    if (it == m_storages.end()) {
      return nullptr;
    }
    return static_cast<Internal::ComponentStorage<T_>*>(it->second.get());
  }

  template <typename T_>
  const Internal::ComponentStorage<T_>* FindStorage() const noexcept {
    auto id = Internal::TypeID<T_>();
    auto it = m_storages.find(id);
    if (it == m_storages.end()) {
      return nullptr;
    }
    return static_cast<const Internal::ComponentStorage<T_>*>(it->second.get());
  }

  // helper: find or create storage for T_
  template <typename T_>
  Internal::ComponentStorage<T_>* EnsureStorage() {
    auto id = Internal::TypeID<T_>();
    auto it = m_storages.find(id);

    if (it != m_storages.end()) {
      return static_cast<Internal::ComponentStorage<T_>*>(it->second.get());
    }

    // create and insert
    auto uptr = std::make_unique<Internal::ComponentStorage<T_>>();
    auto [new_it, inserted] = m_storages.emplace(id, std::move(uptr));
    return static_cast<Internal::ComponentStorage<T_>*>(new_it->second.get());
  }

  // template <typename... Include, typename... Exclude>
  // auto Query();  // More advanced query with excludes
};

}  // namespace SECSY
