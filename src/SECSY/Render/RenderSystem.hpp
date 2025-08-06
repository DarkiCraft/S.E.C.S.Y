#pragma once
#include "Render.hpp"
#include "Sprite.hpp"
#include "../ECS/Registry.hpp"
#include <vector>
#include <tuple>
#include <iostream>
#include <algorithm>

namespace SECSY {

class RenderSystem {
 public:
  RenderSystem(Render& render_) : render(render_) {}

  void Draw(Registry& reg) {
    std::vector<std::tuple<Entity, Sprite*, Position*>> drawables;

    reg.Each<Sprite>([&](Entity e, Sprite& sprite) {
      if (auto pos = reg.GetComponent<Position>(e)) {
        drawables.emplace_back(e, &sprite, pos);
      }
    });
    // std::cout << "Found " << drawables.size() << " drawable entities.\n";
    // sort according to zIndex
    std::sort(
        drawables.begin(), drawables.end(), [](const auto& a, const auto& b) {
          return std::get<1>(a)->zIndex < std::get<1>(b)->zIndex;
        });

    for (auto& [e, sprite, pos] : drawables) {
      if (IsTextureValid(*(sprite->texture))) {
        render.DrawSprite(*sprite, *pos);
      } else {
        // placeholder: draw a rectangle
        std::cout << "Sprite with no texture for entity " << e.id
                  << ", drawing placeholder rectangle.\n";
        render.DrawRectangle(pos->x, pos->y, 64, 64, RED);
      }
    }
  }

 private:
  Render& render;
};
}  // namespace SECSY