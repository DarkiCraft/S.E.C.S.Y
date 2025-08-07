#pragma once

#include <unordered_map>
#include <typeindex>
#include <type_traits>
#include <any>

#include "Entity.hpp"
#include "View.hpp"

namespace SECSY {

class Registry {
 public:
  template <typename T_, typename... Args_>
  T_& Emplace(Entity e_, Args_&&... args_) {
    auto& componentMap = m_components[typeid(T_)];
    T_ instance(std::forward<Args_>(args_)...);
    componentMap[e_] = std::move(instance);
    return std::any_cast<T_&>(componentMap[e_]);
  }

  template <typename T_>
  bool Has(Entity e_) const {
    auto it = m_components.find(typeid(T_));
    if (it == m_components.end()) return false;
    return it->second.find(e_) != it->second.end();
  }

  template <typename T_>
  T_* Get(Entity e_) {
    auto it = m_components.find(typeid(T_));
    if (it == m_components.end()) return nullptr;
    auto& cmap = it->second;
    auto cit   = cmap.find(e_);
    if (cit == cmap.end()) return nullptr;
    return &std::any_cast<T_&>(cit->second);
  }

  template <typename T_>
  void Remove(Entity e_) {
    auto it = m_components.find(typeid(T_));
    if (it != m_components.end()) {
      it->second.erase(e_);
    }
  }

  void Destroy(Entity e_) {
    for (auto& [_, cmap] : m_components) {
      cmap.erase(e_);
    }
  }

  template <typename T>
  View<T> View() const {
    auto it = m_components.find(typeid(T));
    if (it != m_components.end()) {
      return SECSY::View<T>(&it->second);
    }

    return SECSY::View<T>(nullptr);
  }

 private:
  std::unordered_map<std::type_index, std::unordered_map<Entity, std::any>>
      m_components;
};

}  // namespace SECSY
