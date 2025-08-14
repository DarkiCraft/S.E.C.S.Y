#pragma once

#include <cstdint>
#include <utility>

#include <raylib.h>

namespace SECSY {

class Window {
 public:
  // prevent copying
  Window(const Window&)            = delete;
  Window& operator=(const Window&) = delete;

  Window(std::uint32_t width, std::uint32_t height) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    // int m = GetCurrentMonitor();
    // int w = GetMonitorWidth(m);
    // int h = GetMonitorHeight(m);

    InitWindow(width, height, "Window");
    // SetWindowPosition(GetMonitorPosition(m).x, GetMonitorPosition(m).y);
    SetTargetFPS(60);
  }

  std::pair<std::uint32_t, std::uint32_t> Size() {
    return {static_cast<std::uint32_t>(GetScreenWidth()),
            static_cast<std::uint32_t>(GetScreenHeight())};
  }

  bool ShouldClose() const {
    return WindowShouldClose();
  }

  ~Window() {
    CloseWindow();
  }

  // additional methods
};

}  // namespace SECSY