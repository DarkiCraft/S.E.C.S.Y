#pragma once

#include "Entity.hpp"

#include <unordered_map>
#include <memory>
#include <typeindex>

namespace SECSY {

struct IComponentPool {
  virtual void Remove(Entity e_) = 0;
  virtual ~IComponentPool()      = default;
};

template <typename T>
class ComponentPool : public IComponentPool {
 public:
  void Add(Entity e_, const T& component_) {
    m_data[e_] = component_;
  }

  T* Get(Entity e_) {
    auto it = m_data.find(e_);
    return (it != m_data.end()) ? &it->second : nullptr;
  }

  void Remove(Entity e_) override {
    m_data.erase(e_);  // calls destructor of T
  }

  void Each(std::function<void(Entity, T&)> func) {
    for (auto& [entity, comp] : m_data) {
      func(entity, comp);
    }
  }

 private:
  std::unordered_map<Entity, T> m_data;
};

}  // namespace SECSY
