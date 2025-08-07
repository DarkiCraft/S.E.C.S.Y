#include "SECSY/SECSY.hpp"

struct Position {
  float x, y;
};
struct Velocity {
  float dx, dy;
};
struct Health {
  int hp;
};
struct Player {};
struct Enemy {};

void GameScene(SECSY::Registry& reg) {
  // Create player
  SECSY::Entity player = reg.Create();
  reg.Emplace<Position>(player, 10.f, 20.f);
  reg.Emplace<Velocity>(player, 0.f, 0.f);
  reg.Emplace<Health>(player, 100);
  reg.Emplace<Player>(player);

  // Create enemies
  for (int i = 0; i < 5; ++i) {
    SECSY::Entity e = reg.Create();
    reg.Emplace<Position>(e, i * 50.f, 100.f);
    reg.Emplace<Velocity>(e, -1.f, 0.f);
    reg.Emplace<Health>(e, 30);
    reg.Emplace<Enemy>(e);
  }

  // === Update loop ===
  for (auto [entity, pos, vel] : reg.View<Position, Velocity>()) {
    pos.x += vel.dx;
    pos.y += vel.dy;
  }

  // === Damage player if they exist ===
  if (reg.Has<Health>(player)) {
    auto& hp = reg.Get<Health>(player);
    hp.hp -= 10;
    if (hp.hp <= 0) {
      reg.Destroy(player);
    }
  }

  // === Kill all enemies with 0 HP ===
  for (auto [entity, hp] : reg.View<Health>()) {
    if (reg.Has<Enemy>(entity) && hp.hp <= 0) {
      reg.Destroy(entity);
    }
  }
}
