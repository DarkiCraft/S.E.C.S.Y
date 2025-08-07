#pragma once

#include "../ECS/Registry.hpp"
#include "Renderer.hpp"
#include "Components.hpp"

namespace SECSY {

void RenderSystem(Registry& reg) {
  // auto view = reg.Each<Transform, Sprite>();
  // for (auto entity : view) {
  //   auto& tf     = view.get<Transform>(entity);
  //   auto& sprite = view.get<Sprite>(entity);
  //   Renderer::Submit(/*tf, sprite*/);
  // }

  // ^^ Commented until feature/ECS is completed
}

}  // namespace SECSY