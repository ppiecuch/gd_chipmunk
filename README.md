## Chipmunk 2D integration module for Godot Engine

A simple C++ module integrating the Chipmunk 2D physics library into Godot Engine. **Development is still in very early stages, everything is untested.**

- Godot: https://godotengine.org
- Chipmunk 2D: http://chipmunk-physics.net

### Why?

Godot Engine already contains a very good, albeit simple, 2D physics engine implementation. It's based on Chipmunk and, for most games, is more than enough. However, 2D games that require a bit more control and flexibility over its physics could benefit from a bigger and more complex implementation.

### Features

- Integrates Chipmunk2D 7.0.1 into Godot as a C++ module.
- No setup required. If you can build Godot from source, you can use this module.
- Does not disable or replace Godot's 2D physics engine.
- Provides GDScript bindings through `ChipmunkServer`, a singleton not unlike Godot's own `Physics2DServer`.
- ~~Offers Chipmunk functionality through custom Nodes~~ (Not yet implemented)

### How to Use

- Clone the contents of this repository into `<Your Godot source folder>/modules/chipmunk`
  - Please note that this project is meant to be used with Godot's **2.1** branch.
- Build and run Godot normally

### License

Released under the [MIT License](LICENSE).

### To Do

See [TODO](TODO.md).
