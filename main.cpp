#include "SECSY/Platform/Window.hpp"
#include "SECSY/Render/Renderer.hpp"

#include <iostream>

int main() {
  Image image = GenImageColor(16, 16, RED);  // 16x16 red square

  SECSY::Window window(640, 360);
  SECSY::Renderer renderer(320, 180);

  Texture2D texture =
      LoadTextureFromImage(image);  // textures must be initialized AFTER window

  UnloadImage(image);

  // this will be part of out internal game loop LATER LATER LATER (very later)
  while (!window.ShouldClose()) {
    renderer.Begin();

    // this will be internalized in RenderSystem AFTER ECS is fixed
    renderer.Submit({texture, {100, 100}, 0, {1, 1}, WHITE, 1});

    renderer.End();
  }

  UnloadTexture(texture);
}