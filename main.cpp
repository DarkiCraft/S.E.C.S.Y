#include "SECSY/SECSY.hpp"

struct Position {
  float x;
  float y;
};

int main() {
  SECSY::Entity e1{1, 1};
  SECSY::Entity e2{2, 1};

  SECSY::Registry reg;
  reg.Emplace<Position>(e1, Position{1, 2});
  reg.Emplace<Position>(e2, Position{3, 4});

  for (auto [entity, pos] : reg.View<Position>()) {
  }
}
