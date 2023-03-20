# NoEngine

NoEngine is a lightweight, primarily 2D, game engine for use in my own personal projects. It currently uses [Tiled](mapeditor.org) as a map editor, [Aseprite](https://www.aseprite.org/) for tilesets and spritesheets, and most bitmap font formats for text rendering.

NoEngine was built with [EnTT](https://github.com/skypjack/entt) and makes use of the entity-component-system pattern for data management and organization. Much of the original reason for this project was to learn about this architectural pattern.

The reason for the name NoEngine is that, in reality, this is much less a game engine than it is a game *template*, that is tailored to my own needs. There is no library to link against and no api for a front-end.

In the future, I may turn this into something similar to MonoGame, but that is not part of my current plams.

## Features

+ Modern OpenGL rendering
  + VBOs, VAOs, FBOs, etc.
  + Instanced rendering for all quads
+ Runtime texture atlas generation for spritesheets and tilesets
  + Includes simple texture generation and loading
+ Collision detection using efficient spacial lookups
+ Aseprite spritesheet and animation loading
+ Tiled map loading
+ Bitmap font rendering

*Because the primary purpose of this game engine is for my own use, there is little in terms of readable documentation at this point. If you have any questions, feel free to reach out.*

## Libraries/Tools

+ [EnTT](https://github.com/skypjack/entt)
  + Entity-component-system
+ SDL2
  + Windowing
  + OpenGL context
+ GLM
  + Math
+ [rectpack2D](https://github.com/TeamHypersomnia/rectpack2D)
  + Fast rectangle packing algorithm for texture atlas
+ stb_image.h
  + Image file loading
+ [rapidjson](http://rapidjson.org/)
  + JSON loading
+ [tinytmx](https://github.com/KaseyJenkins/tinytmx)
  + Tiled map loader
+ [FreeType](http://freetype.org/)
  + Font loading and rasterization
+ [lightgrid](https://github.com/jakehffn/lightgrid)
  + Spacial lookup data-structure
  + *(My own library)*
