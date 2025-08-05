#pragma once

namespace SECSY {

class Registry;  // Forward declaration

class ISystem {
 public:
  virtual ~ISystem()                                = default;
  virtual void Update(Registry& registry, float dt) = 0;
};

}  // namespace SECSY
