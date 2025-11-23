# PhezuEngine

**A lightweight 2D Game Engine built in C++ with C# scripting.**  
Created as a personal learning project to understand engine architecture. There is a strong focus to keep the codebase as simple and readable as possible so others can use this as a learning resource.

---

## Features
- **C# scripting** through Mono integration  
- **Prefab system** with nesting and overrides
- **Physics system** (axis-aligned boxes only)
- **Asset Management** of scene and prefab files
- **Basic rendering** of colored boxes  
- **Minimal dependencies** and clean, readable architecture

## Goals
- Make engine programming **easy and accessible** for learners  
- Enable creation of **simple classic games** like *Pac-Man, Breakout, Tetris, Snake,* or *Pocket Tanks*  
- Keep the **codebase simple and readable**  
- Maintain **as few dependencies as possible**

## Dependencies
- [Mono](https://www.mono-project.com/) Used for scripting
- [SDL2](https://www.libsdl.org/) Used for rendering, windows and input (Planned to be removed)

## Technical Overview
- Written in **C++17**
- Uses **CMake** for cross-platform builds  
- Mono integration for **runtime C# scripting**  
- Has complete separation of project and engine files.
  
Project consists of 4 subprojects:
- Phezu - This is the static library containing the engine code.
- Runtime - This is the executable that links to Phezu and launches the engine.
- ScriptCore - This is the C# dll containing all of the engine classes.
- Editor
  - EditorShell - (This will also be an executable which will link to Phezu and run editor commands)
  - EditorGUI - (This will use imgui and EditorShell to run the editor app)

## CI / Tested Configurations

- Windows x64 — MSVC Debug
- Windows x64 — MSVC Release

---

## Windows Setup
**Requirements**
- Visual Studio installation  

**Instructions**
```bash
1. Clone or download the repository
2. Double-click Setup_Win.bat
```

## macOS Setup (Experimental)
> ⚠️ macOS support is **not officially maintained** and may require manual setup.

**Requirements**
```bash
brew install mono
```

**Instructions**
```bash
1. Open Terminal at the root directory
2. Run ./Setup_Mac.sh
```

⚠️ *Tested in a local development environment, but not verified in a production or release setup.*

## Linux not currently supported

## Roadmap
- Separate DLLs for engine and user scripts  
- Remove SDL2 dependency  
- Add support for primitives and rotation  
- Sprite rendering
- Linux support
- Editor application (Much Later)

---

## Demo
![BreakoutPreview](./BreakoutPreview.png)
> *This is a preview of the sample breakout game.*

