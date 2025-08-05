#include <iostream>
#include <raylib.h>

#include "SECSY/SECSY.hpp"

struct Position {
  float x, y;
};

struct Velocity {
  float dx, dy;
};

struct Name {
  std::string value;
  ~Name() {
    std::cout << "Destroying Name: " << value << "\n";
  }
};

/**
 * @brief Demonstrates basic entity-component-system (ECS) usage with rendering and input handling.
 *
 * Creates entities with position, velocity, and name components, updates positions, and renders a window that can toggle between fullscreen and windowed modes. Outputs entity information to the console and manages entity lifecycles.
 *
 * @return int Exit status code.
 */
int main() {
  SECSY::Registry reg;
  SECSY::Render render(800, 600, "SECSY Example");

  SECSY::Entity e1 = reg.CreateEntity();
  SECSY::Entity e2 = reg.CreateEntity();

  reg.AddComponent(e1, Position{10.0f, 20.0f});
  reg.AddComponent(e1, Velocity{1.0f, 1.5f});

  reg.AddComponent(e2, Position{5.0f, 2.0f});
  reg.AddComponent(e2, Name{"SECSY"});

  reg.Each<Position>([&](SECSY::Entity e, Position& pos) {
    if (auto vel = reg.GetComponent<Velocity>(e)) {
      pos.x += vel->dx;
      pos.y += vel->dy;
    }
    std::cout << "Entity " << e.id << " Position: " << pos.x << ", " << pos.y
              << "\n";
  });

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (render.isFullscreen()) {
      DrawText("Fullscreen Mode", 10, 10, 20, DARKGRAY);
    } else {
      DrawText("Windowed Mode", 10, 10, 20, DARKGRAY);
    }

    EndDrawing();

    if (IsKeyPressed(KEY_F11)) {
      render.ToggleFullscreenMode();
    }
  }

  reg.Each<Name>([](SECSY::Entity e, Name& n) {
    std::cout << "Entity[" << e.id << "]: " << n.value << "\n";
  });

  reg.DestroyEntity(e2);
}
