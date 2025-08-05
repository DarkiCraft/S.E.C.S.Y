#pragma once

#include <cstdint>
#include <string>
#include "cmath"
#include "raylib.h"
#include "Sprite.hpp"

namespace SECSY {
enum class ScalingMode {
  LETTERBOX,      // Maintain aspect ratio, letterbox if necessary
  PIXEL_PERFECT,  // Integer scale, no letterboxing
};
class Render {
  // class responsible for creating a screen
 public:
  Render(unsigned int width, unsigned int height, std::string title)
      : width(width),
        height(height),
        title(std::move(title)),
        scalingMode(ScalingMode::LETTERBOX),
        renderScale(1.0f),
        renderOffset{0.0f, 0.0f} {
    InitWindow(this->width, this->height, this->title.c_str());
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    renderTexture = LoadRenderTexture(width, height);
    SetTargetFPS(60);
  }
  ~Render() {
    UnloadRenderTexture(renderTexture);
    CloseWindow();
  }

  void ToggleFullscreenMode() {
    ToggleFullscreen();
  }

  void SetWindowSize(int width, int height) {
    if (!IsWindowFullscreen()) {
      this->width  = width;
      this->height = height;
      UpdateScaling();
    }
  }

  bool isFullscreen() const {
    return IsWindowFullscreen();
  }

  void SetScalingMode(ScalingMode mode) {
    scalingMode = mode;
    UpdateScaling();
  }

  void BeginRendering() {
    if (GetScreenWidth() != currentWindowWidth ||
        GetScreenHeight() != currentWindowHeight) {
      UpdateScaling();
    }

    BeginTextureMode(renderTexture);
    ClearBackground(RAYWHITE);
  }

  // Textures + Sprites
  void DrawSprite(const Sprite& sprite) {
    Rectangle destRect = {sprite.position.x + renderOffset.x,
                          sprite.position.y + renderOffset.y,
                          sprite.sourceRect.width * renderScale,
                          sprite.sourceRect.height * renderScale};

    DrawTexturePro(*(sprite.texture),
                   sprite.sourceRect,
                   destRect,
                   sprite.origin,
                   sprite.rotation,
                   sprite.color);
  }

  void DrawRectangle(float x, float y, float width, float height, Color color) {
    DrawRectangle(x * renderScale + renderOffset.x,
                  y * renderScale + renderOffset.y,
                  width * renderScale,
                  height * renderScale,
                  color);
  }

  void DrawText(
      const std::string& text, float x, float y, int fontSize, Color color) {
    DrawText(text.c_str(),
             x * renderScale + renderOffset.x,
             y * renderScale + renderOffset.y,
             fontSize * renderScale,
             color);
  }

  void DrawCircle(float x, float y, float radius, Color color) {
    DrawCircle(x * renderScale + renderOffset.x,
               y * renderScale + renderOffset.y,
               radius * renderScale,
               color);
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