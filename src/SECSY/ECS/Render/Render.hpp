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
  /**
   * @brief Constructs a resizable window with the specified dimensions and title.
   *
   * Initializes the rendering window, sets it to be resizable, and targets 60 frames per second.
   *
   * @param width The initial width of the window in pixels.
   * @param height The initial height of the window in pixels.
   * @param title The title displayed on the window.
   */
  Render(unsigned int width, unsigned int height, std::string title)
      : width(width), height(height), title(std::move(title)) {
    InitWindow(this->width, this->height, this->title.c_str());
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);
  }
  /**
   * @brief Destroys the Render object and closes the window.
   *
   * Releases all resources associated with the window.
   */
  ~Render() {
    CloseWindow();
  }

  /**
   * @brief Toggles the window between fullscreen and windowed mode.
   */
  void ToggleFullscreenMode() {
    ToggleFullscreen();
  }

  /**
   * @brief Checks if the window is currently in fullscreen mode.
   *
   * @return true if the window is fullscreen, false otherwise.
   */
  bool isFullscreen() const {
    return IsWindowFullscreen();
  }

  /**
   * @brief Sets the window's rendering scaling mode.
   *
   * Changes the scaling mode used for rendering and updates scaling parameters to reflect the new mode.
   *
   * @param mode The desired scaling mode (LETTERBOX or PIXEL_PERFECT).
   */
  void SetScalingMode(ScalingMode mode) {
    scalingMode = mode;
    UpdateScaling();
  }

  /**
   * @brief Checks for window size changes and updates rendering scale and offset if needed.
   *
   * Should be called at the start of each frame to ensure the rendered content is properly scaled and centered when the window is resized.
   */
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

  /**
   * @brief Updates the render scale and offset based on the current window size and scaling mode.
   *
   * Recalculates the scaling factor and offset to ensure the rendered content is properly scaled and centered within the window, according to the selected scaling mode.
   */
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