#pragma once

#include <cstdint>
#include <string>
#include "raylib.h"

namespace SECSY {
enum class ScalingMode {
  LETTERBOX,      // Maintain aspect ratio, letterbox if necessary
  PIXEL_PERFECT,  // Integer scale, no letterboxing
};
class Render {
  // class responsible for creating a screen
 public:
  Render(unsigned int width, unsigned int height, std::string title)
      : width(width), height(height), title(std::move(title)) {
    InitWindow(this->width, this->height, this->title.c_str());
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);
  }
  ~Render() {
    CloseWindow();
  }

  void ToggleFullscreenMode() {
    ToggleFullscreen();
  }

  bool isFullscreen() const {
    return IsWindowFullscreen();
  }

 private:
  std::int32_t width;
  std::int32_t height;
  std::string title;
};
}  // namespace SECSY