# PhezuEngine

**A lightweight 2D Game Engine built in C++ with C# scripting.**  
Created as a personal learning project to understand engine architecture. There is a strong focus to keep the codebase as simple and readable as possible so others can use this as a learning resource.

---

## Features
- **C# scripting** through Mono integration  
- **Prefab system** with nesting and overrides
- **Physics system** (axis-aligned boxes only)
- **Asset Management** of scene and prefab files
- **Custom Rendering** of sprites  
- **Minimal dependencies** and clean, readable architecture

## Goals
- Make engine programming **easy and accessible** for learners  
- Enable creation of **simple classic games** like *Pac-Man, Breakout, Tetris, Snake,* or *Pocket Tanks*  
- Keep the **codebase simple and readable**  
- Maintain **as few dependencies as possible**

## Dependencies
- [Mono](https://www.mono-project.com/) Used for scripting

## Technical Overview
- Written in **C++17**.
- Uses **CMake** for cross-platform builds.
- Mono integration for **runtime C# scripting**.
- Custom **OpenGL** rendering backend.
- List of currently supported assets:
    - Scene
    - Prefab
    - Image
    - Texture
    - Shader
    - Material
    - Mesh
  
Project consists of 4 subprojects:
- Phezu - This is the static library containing the engine code.
- Runtime - This is the executable that links to Phezu and launches the engine.
- ScriptCore - This is the C# dll containing all of the engine classes.
- Editor
  - EditorShell - (This is an executable which will link to Phezu and run editor commands)
  - EditorGUI - (This will use imgui and EditorShell to run the editor app)
 
List of all third party code used (Mostly single headers):
  - glm
  - nlohmann
  - stb_image
  - tinyxml2
  - glad

## CI / Tested Configurations

- Windows x64 — MSVC Debug
- Windows x64 — MSVC Release

---

## Windows Setup
**Requirements**
```bash
  Visual Studio installation
  Supported versions: 2017, 2019, 2022, 2026
  Desktop development with C++ Module is required
```

**Instructions**
```bash
1. Clone or download the repository
2. Run Setup_Win.bat
```

## Linux and Mac not currently supported

## Current Roadmap 
- Add a documentation site.
- Add custom physics module.
- Android support (Much Later).
- Editor application (Much Later).

---

## Demo
![BreakoutPreview](./BreakoutPreview.png)
> *This is a preview of the sample breakout game.*

