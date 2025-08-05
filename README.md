# Simple Entity Component System for You (S.E.C.S.Y)

**S.E.C.S.Y** is a lightweight, modular 2D game engine framework built around a custom ECS (Entity Component System) architecture with performance and extensibility in mind.

This repository contains the core engine code. It is designed to be used as a library by external games or tools.

## 📁 Directory Overview

Each top-level folder corresponds to a self-contained module or domain in the engine.

### `Audio/`

Handles all audio-related functionality:

* Music and sound effect playback
* Audio emitters and listeners (for 2D positional audio)
* Volume, pitch, and spatialization controls

### `Core/`

Low-level utilities and engine-wide types:

* Engine lifecycle (init, run, shutdown)
* Logging
* Time / delta-time management
* Global configuration
* Error handling

### `ECS/`

Custom ECS implementation:

* Entity creation, destruction
* Component storage and access
* System scheduling (if implemented)
* Query and iteration logic

Minimal runtime overhead. Zero polymorphism. Pure C++17.

### `Math/`

Mathematics primitives and helpers:

* Vectors, matrices, transforms
* Geometry utilities
* Collision math

### `Physics/`

2D physics simulation:

* Rigid body dynamics
* Collision detection and resolution
* Integration with ECS (optional)
* Basic forces (gravity, drag)

Not a full Box2D — just enough for most arcade-style 2D games.

### `Platform/`

Platform abstraction layer:

* Input (keyboard, mouse, gamepad)
* Filesystem access
* Window and monitor info
* Misc OS interactions

Wraps around raylib or other backend as needed.

### `Render/`

All rendering-related logic:

* Sprite batching
* Text rendering
* Cameras and viewports
* Render layers and sorting
* ECS-driven rendering systems

Backed by raylib under the hood.

### `Scene/`

Scene and prefab management:

* Scene loading/unloading
* Prefab definitions (composition of entities/components)
* Runtime entity spawning
* Possibly serialization

Optional: may integrate with scripting or editor tools later.


## 📄 SECSY.hpp

Umbrella header for including the full engine in one go:

```cpp
#include <SECSY/SECSY.hpp>
```

Useful for external games or projects that want to import the entire engine API without diving into internal layout.

## Authors

This project is maintained and developed by the following people:

- [@DarkiCraft](https://github.com/DarkiCraft)
- [@redsteadz](https://github.com/redsteadz)

## License

This project is distributed under the [MIT License](LICENSE).