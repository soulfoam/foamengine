# About

Foam Engine is a 3D C game engine. It is still incomplete in a lot of ways, but the core engine is mainly there. The editor is still a WIP as well and bare bones, but there is plenty of code to look through core engine wise! It's not really ready for a public release yet, so this repo will be insanely light as far as documentation and everything else. 

The are only a couple (mostly common) dependencies. SDL2 is used for windowing, and OpenGL is also used for rendering. For physics, no libaries are used. I based the physics off this paper https://www.peroxide.dk/papers/collision/collision.pdf. Assimp is used in the editor/tooling to convert model/animation/skeleton data into a custom format for faster loading. Most of the logic is running through the ECS, so most things are systems and components.

Besides the core foam(engine) folder, there is also the game folder which is a project that is using the engine.

# Screenshots

The engine supports lighting but these screenshots don't contain scenes with any lighting for the most part.

Screenshots/GIFS will be posted soon, I just wanted the code repo up.


