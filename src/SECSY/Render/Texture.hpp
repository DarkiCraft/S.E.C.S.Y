#pragma once

// TextureManager that manages textures for rendering
// Avoiding duplicate textures and providing easy access to them for sprites

#include <cstdint>
#include <string>
#include "raylib.h"
#include <unordered_map>
#include <memory>

namespace SECSY {
class TextureManager {
 public:
  static std::shared_ptr<Texture2D> LoadTexture(const std::string& filePath) {
    auto it = textures.find(filePath);
    if (it != textures.end()) {
      return it->second;
    }

    Texture2D texture = ::LoadTexture(filePath.c_str());
    if (texture.id == 0) {
      throw std::runtime_error("Failed to load texture: " + filePath);
    }
    auto texturePtr = std::shared_ptr<Texture2D>(
        new Texture2D(std::move(texture)), [](Texture2D* t) {
          ::UnloadTexture(*t);
          delete t;
        });

    textures[filePath] = texturePtr;
    return texturePtr;
  }

 private:
  static inline std::unordered_map<std::string, std::shared_ptr<Texture2D>>
      textures;
};
}  // namespace SECSY