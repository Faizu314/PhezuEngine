# PhezuEngine Roadmap

Here is a list of planned features. The major features and their breakdowns are listed in order of priority from highest to lowest. Note that all of this is subject to change...

## 2D Physics Engine

A custom 2D physics engine with collision detection and resolution for primitive 2D shapes and polygons.

## Editor Features

- Backend Features
    - Prefab/Scene file editing
    - Script/Shader file editing
    - Pixel Art Sprite Editor
    - Mesh Editor for 2D custom polygon
- ImGUI based GUI application
    - Scene View
    - Game View
    - Inspector Tab
    - Scene Hierarchy Tab
    - Console Tab
    - Project/Directory Tab

## 2D Graphics

- Font Rendering
- Primitive Shapes Rendering

## Scripting

I am considering to remove Mono as an external dependency and implement a custom scripting language. This is to stick to the manifesto of having minimal external dependencies. Mono is a huge third party library and adds to the installation and build process. It adds a lot of unwanted files in the editor installation and the final build of the game increasing the storage footprint.

## Platform

- Android
- Linux
- Mac (Maybe)

## Rendering

- OpenGL ES Renderer
- Vulkan (Maybe)