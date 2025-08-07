#pragma once

#include <unordered_map>
#include <utility>

namespace SECSY {

template <typename T>
class View {
 public:
  using MapType = std::unordered_map<SECSY::Entity, std::any>;

  View(const MapType* map) : m_map(map) {}

  class Iterator {
   public:
    using InnerIter = typename MapType::const_iterator;

    Iterator(InnerIter it) : m_it(it) {}

    std::pair<SECSY::Entity, const T&> operator*() const {
      return {m_it->first, std::any_cast<const T&>(m_it->second)};
    }

    Iterator& operator++() {
      ++m_it;
      return *this;
    }

    bool operator!=(const Iterator& other) const {
      return m_it != other.m_it;
    }

   private:
    InnerIter m_it;
  };

  Iterator begin() const {
    return m_map ? Iterator(m_map->begin()) : Iterator({});
  }

  Iterator end() const {
    return m_map ? Iterator(m_map->end()) : Iterator({});
  }

 private:
  const MapType* m_map;
};

}  // namespace SECSY