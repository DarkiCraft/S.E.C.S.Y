#include "Test.hpp"

#include <cstdint>
#include <string_view>
#include <raylib-cpp.hpp>

namespace Test {

void Foo() {
  const std::uint32_t screen_width  = 800;
  const std::uint32_t screen_height = 450;

  raylib::Window window(screen_width, screen_height, "Foo");

  const std::uint32_t fps = 60;

  SetTargetFPS(fps);

  while (!window.ShouldClose()) {
    BeginDrawing();

    window.ClearBackground(RAYWHITE);

    const std::string_view msg     = "Foo";
    const std::uint32_t font_size  = 40;
    const std::uint32_t text_width = MeasureText(msg.data(), font_size);

    DrawText(msg.data(),
             (screen_width - text_width) / 2,
             (screen_height - font_size) / 2,
             font_size,
             BLACK);

    EndDrawing();
  }
}

}  // namespace Test
