#pragma once
#include <cstdint>
#include <string>
#include "raylib.h"
#include "Texture.hpp"

namespace SECSY {
class Sprite {
 public:
  std::shared_ptr<Texture2D> texture;
  Rectangle sourceRect;
  Vector2 origin;
  float rotation;
  Color color;
  int zIndex;
  std::string filePath;

 public:
  Sprite()
      : texture(nullptr),
        sourceRect{0, 0, 0, 0},
        origin{0, 0},
        rotation(0),
        color(WHITE),
        zIndex(0) {}

  Sprite(const std::string& filePath, int z = 0)
      : texture(SECSY::TextureManager::LoadTexture(filePath)), zIndex(z) {
    sourceRect = {0,
                  0,
                  static_cast<float>(texture->width),
                  static_cast<float>(texture->height)};
    origin     = {0.0f, 0.0f};
    rotation   = 0.0f;
    color      = WHITE;
  }
};
}  // namespace SECSY