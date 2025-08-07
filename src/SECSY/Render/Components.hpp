#pragma once

#include <cstdint>

#include <raylib-cpp.hpp>  // or C

namespace SECSY {

struct Sprite {
  raylib::Texture2D texture;  // or C
  raylib::Rectangle source;   // or C
  raylib::Color tint;         // or C
  std::int32_t layer;         // z-index
};

}  // namespace SECSY