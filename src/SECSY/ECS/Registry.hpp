#pragma once

#include "Entity.hpp"
#include "ComponentPool.hpp"

#include <unordered_map>
#include <typeindex>
#include <memory>
#include <vector>

namespace SECSY {

class Registry {
 public:
  Entity CreateEntity() {
    Entity e;
    e.id = m_next_id++;
    if (e.id >= m_generations.size()) {
      m_generations.resize(e.id + 1);
    }
    e.generation = m_generations[e.id];
    return e;
  }

  void DestroyEntity(Entity e_) {
    m_generations[e_.id]++;
    for (auto& [_, pool] : m_pools) {
      pool->Remove(e_);
    }
  }

  template <typename T>
  void AddComponent(Entity e_, const T& component) {
    GetOrCreatePool<T>()->Add(e_, component);
  }

  template <typename T>
  T* GetComponent(Entity e_) {
    auto* pool = GetPool<T>();
    return pool ? pool->Get(e_) : nullptr;
  }

  template <typename T>
  void Each(std::function<void(Entity, T&)> func_) {
    auto* pool = GetPool<T>();
    if (pool) {
      pool->Each(func_);
    }
  }

 private:
  uint32_t m_next_id = 1;
  std::vector<uint32_t> m_generations;

  std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> m_pools;

  template <typename T>
  ComponentPool<T>* GetPool() {
    auto it = m_pools.find(typeid(T));
    if (it == m_pools.end()) return nullptr;
    return static_cast<ComponentPool<T>*>(it->second.get());
  }

  template <typename T>
  ComponentPool<T>* GetOrCreatePool() {
    auto* pool = GetPool<T>();
    if (!pool) {
      auto new_pool      = std::make_unique<ComponentPool<T>>();
      pool               = new_pool.get();
      m_pools[typeid(T)] = std::move(new_pool);
    }
    return pool;
  }
};

}  // namespace SECSY
