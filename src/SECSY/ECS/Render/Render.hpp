#pragma once

#include <cstdint>
#include <string>
#include "cmath"
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

  void SetScalingMode(ScalingMode mode) {
    scalingMode = mode;
    UpdateScaling();
  }

  void BeginRendering() {
    if (!WindowShouldClose() && (GetScreenWidth() != currentWindowWidth ||
                                 GetScreenHeight() != currentWindowHeight)) {
      UpdateScaling();
    }
  }

 private:
  std::int32_t width;
  std::int32_t currentWindowWidth;
  std::int32_t currentWindowHeight;
  std::int32_t height;
  std::string title;

  ScalingMode scalingMode;
  float renderScale;
  Vector2 renderOffset;
  RenderTexture2D renderTexture;

  void UpdateScaling() {
    currentWindowWidth  = GetScreenWidth();
    currentWindowHeight = GetScreenHeight();

    float scaleX = (float)currentWindowWidth / width;
    float scaleY = (float)currentWindowHeight / height;

    switch (scalingMode) {
      case ScalingMode::LETTERBOX:
        renderScale = std::min(scaleX, scaleY);
        break;

      case ScalingMode::PIXEL_PERFECT:
        renderScale = std::max(1.0f, std::floor(std::min(scaleX, scaleY)));
        break;
    }

    float scaledWidth  = width * renderScale;
    float scaledHeight = height * renderScale;

    renderOffset.x = (currentWindowWidth - scaledWidth) / 2.0f;
    renderOffset.y = (currentWindowHeight - scaledHeight) / 2.0f;
  }
};
}  // namespace SECSY