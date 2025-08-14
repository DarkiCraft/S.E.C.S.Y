#pragma once

#include <algorithm>
#include <cstdint>
#include <deque>

#include <raylib.h>

// this is temporary, we'll fix this when the ECS is cooked
struct SpriteDrawCommand {
  Texture2D texture;
  Vector2 position;
  float rotation;
  Vector2 scale;
  Color tint;
  int layer;  // z-index for sorting
};

namespace SECSY {

class Renderer {
 public:
  // prevent copying
  Renderer(const Renderer&)            = delete;
  Renderer& operator=(const Renderer&) = delete;

  Renderer(std::uint32_t internal_width, std::uint32_t internal_height) {
    m_internal_width  = internal_width;
    m_internal_height = internal_height;
    m_target          = LoadRenderTexture(internal_width, internal_height);
  }

  void Begin() {
    BeginTextureMode(m_target);
    ClearBackground(WHITE);  // Or whatever clear color
  }

  void End() {
    // Sort draw queue by layer (ascending)
    std::sort(m_draw_queue.begin(),
              m_draw_queue.end(),
              [](const auto& a, const auto& b) { return a.layer < b.layer; });

    for (const auto& cmd : m_draw_queue) {
      DrawTexturePro(
          cmd.texture,
          {0, 0, (float)cmd.texture.width, (float)cmd.texture.height},
          {cmd.position.x,
           cmd.position.y,
           cmd.texture.width * cmd.scale.x,
           cmd.texture.height * cmd.scale.y},
          {0, 0},
          cmd.rotation,
          cmd.tint);
    }

    EndTextureMode();

    m_draw_queue.clear();

    // Now render the final texture scaled to the actual screen
    ClearBackground(BLACK);  // Or whatever outer color

    float windowWidth  = GetScreenWidth();
    float windowHeight = GetScreenHeight();

    float scale_x = windowWidth / static_cast<float>(m_internal_width);
    float scale_y = windowHeight / static_cast<float>(m_internal_height);
    float scale   = std::min(scale_x, scale_y);

    float offset_x = (windowWidth - m_internal_width * scale) / 2.0f;
    float offset_y = (windowHeight - m_internal_height * scale) / 2.0f;

    DrawTexturePro(
        m_target.texture,
        {0, 0, (float)m_target.texture.width, -(float)m_target.texture.height},
        {offset_x,
         offset_y,
         m_internal_width * scale,
         m_internal_height * scale},
        {0, 0},
        0.0f,
        WHITE);
  }

  void Submit(const SpriteDrawCommand& cmd) {
    m_draw_queue.push_back(cmd);
  }

 private:
  RenderTexture2D m_target;
  std::deque<SpriteDrawCommand> m_draw_queue;

  std::uint32_t m_internal_width;
  std::uint32_t m_internal_height;
};

// note: apparently Begin() and End() should be part of Platform???

}  // namespace SECSY