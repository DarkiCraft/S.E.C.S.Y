#include <iostream>
#include <raylib.h>

#include "SECSY/SECSY.hpp"

// struct Position {
//   float x, y;
// };

// struct Velocity {
//   float dx, dy;
// };

struct Name {
  std::string value;
  ~Name() {
    std::cout << "Destroying Name: " << value << "\n";
  }
};

int main() {
  SECSY::Registry reg;
  SECSY::Render render(800, 600, "SECSY Example");
  SECSY::RenderSystem renderSystem(render);

  SECSY::Entity e1 = reg.CreateEntity();
  SECSY::Entity e2 = reg.CreateEntity();

  reg.AddComponent(e1, Position{100.0f, 100.0f});
  reg.AddComponent(e1, Velocity{1.0f, 1.5f});
  reg.AddComponent(e1, SECSY::Sprite("assets/sprite.jpeg"));

  reg.AddComponent(e2, Position{5.0f, 2.0f});
  reg.AddComponent(e2, Name{"SECSY"});

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    render.BeginRendering();
    renderSystem.Draw(reg);
    render.EndRendering();

    if (render.isFullscreen()) {
      DrawText("Fullscreen Mode", 10, 10, 20, DARKGRAY);
    } else {
      DrawText("Windowed Mode", 10, 10, 20, DARKGRAY);
    }

    reg.Each<Position>([&](SECSY::Entity e, Position& pos) {
      if (auto vel = reg.GetComponent<Velocity>(e)) {
        pos.x += vel->dx;
        pos.y += vel->dy;
      }
      // If the position goes out of bounds, invert the velocity
      if (pos.x < 0) pos.x += 800;
      if (pos.x > 800) pos.x -= 800;
      if (pos.y < 0) pos.y += 600;
      if (pos.y > 600) pos.y -= 600;
      std::cout << "Entity " << e.id << " Position: " << pos.x << ", " << pos.y
                << "\n";
    });

    EndDrawing();

    if (IsKeyPressed(KEY_F11)) {
      render.ToggleFullscreenMode();
    }
  }

  reg.Each<Name>([](SECSY::Entity e, Name& n) {
    std::cout << "Entity[" << e.id << "]: " << n.value << "\n";
  });

  reg.DestroyEntity(e2);
  return 0;
}
