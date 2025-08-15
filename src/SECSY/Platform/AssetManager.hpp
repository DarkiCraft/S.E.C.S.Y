#pragma once
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <stdexcept>
#include <functional>

class AssetManager {
 public:
  AssetManager() = default;
  ~AssetManager() {
    UnloadAll();
  }

  template <typename T>
  void RegisterType(std::function<T(const std::string&)> loader,
                    std::function<void(T&)> unloader) {
    TypeData typeData;
    typeData.loader = [loader](const std::string& path) {
      return std::make_shared<T>(loader(path));
    };
    typeData.unloader = [unloader](std::shared_ptr<void> ptr) {
      unloader(*std::static_pointer_cast<T>(ptr));
    };
    typeData.assets                   = {};
    types[std::type_index(typeid(T))] = std::move(typeData);
  }

  template <typename T>
  T& Get(const std::string& path) {
    auto& typeData = GetTypeData<T>();

    auto it = typeData.assets.find(path);
    if (it != typeData.assets.end()) {
      return *std::static_pointer_cast<T>(it->second);
    }

    auto assetPtr         = typeData.loader(path);
    typeData.assets[path] = assetPtr;
    return *std::static_pointer_cast<T>(assetPtr);
  }

  template <typename T>
  void Unload(const std::string& path) {
    auto& typeData = GetTypeData<T>();
    auto it        = typeData.assets.find(path);
    if (it != typeData.assets.end()) {
      typeData.unloader(it->second);
      typeData.assets.erase(it);
    }
  }

  void UnloadAll() {
    for (auto& [type, typeData] : types) {
      for (auto& [path, asset] : typeData.assets) {
        typeData.unloader(asset);
      }
      typeData.assets.clear();
    }
  }

 private:
  struct TypeData {
    std::function<std::shared_ptr<void>(const std::string&)> loader;
    std::function<void(std::shared_ptr<void>)> unloader;
    std::unordered_map<std::string, std::shared_ptr<void>> assets;
  };

  std::unordered_map<std::type_index, TypeData> types;

  template <typename T>
  TypeData& GetTypeData() {
    auto it = types.find(std::type_index(typeid(T)));
    if (it == types.end()) {
      throw std::runtime_error("Asset type not registered!");
    }
    return it->second;
  }
};
