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
  Vector2 position;
  Vector2 origin;
  float rotation;
  Color color;

 public:
  Sprite(const std::string& filePath)
      : texture(SECSY::TextureManager::LoadTexture(filePath)) {
    sourceRect = {0,
                  0,
                  static_cast<float>(texture->width),
                  static_cast<float>(texture->height)};
    position   = {0.0f, 0.0f};
    origin     = {0.0f, 0.0f};
    rotation   = 0.0f;
    color      = WHITE;
  }
};
}  // namespace SECSY