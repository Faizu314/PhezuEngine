# PhezuEngine

**A lightweight 2D Game Engine built in C++ with C# scripting.**  
Created as a personal learning project to understand engine architecture while keeping the code simple and educational.

---

## 🚀 Features
- **C# scripting** through Mono integration  
- **Prefab system** with nesting and overrides
- **Physics system** (axis-aligned boxes only)
- **Asset Management** of scene and prefab files
- **Basic rendering** of colored boxes  
- **Minimal dependencies** and clean, readable architecture

---

## 📚 Goals
- Make engine programming **easy and accessible** for learners  
- Enable creation of **simple classic games** like *Pac-Man, Breakout, Tetris, Snake,* or *Pocket Tanks*  
- Keep the **codebase simple and readable**  
- Maintain **as few dependencies as possible**

---

## 🧩 Dependencies
- [Mono](https://www.mono-project.com/) Used for scripting
- [SDL2](https://www.libsdl.org/) Used for rendering, windows and input (Planned to be removed)  

---

## 💻 Windows Setup
**Requirements**
- Visual Studio installation  

**Instructions**
```bash
1. Clone or download the repository
2. Double-click Setup_Win.bat
```

---

## 🍎 macOS Setup (Experimental)
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

---

## 🗺️ Roadmap
- [ ] Separate DLLs for engine and user scripts  
- [ ] Remove SDL2 dependency  
- [ ] Add support for primitives and rotation  
- [ ] Sprite rendering
- [ ] Editor application (Much Later)

---

## 🧠 Technical Overview
- Written in **C++17**
- Uses **CMake** for cross-platform builds  
- Mono integration for **runtime C# scripting**  
- Has complete separation of project and engine files.  

---

## 🎮 Demo
> *(Add a screenshot or GIF of the Breakout game here — even a still image of the running game window works wonders.)*

---

## 🤝 About the Project
This project is part of my personal journey into engine development.  
I’m sharing it publicly so that others can explore and learn from it.

