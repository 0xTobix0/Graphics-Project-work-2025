<<<<<<< HEAD
# Graphics-Project-work-2025
# The Luminous Field - Computer Graphics Project

This is a computer graphics project implementing an infinite scene with advanced graphics features using OpenGL 3.3.

## Project Requirements
- C++17 or later
- OpenGL 3.3
- GLFW3

## Build Instructions

1. Clone the repository:
```bash
git clone https://github.com/0xTobix0/Graphics-Project-work-2025.git
cd Graphics-Project-work-2025
```

2. Build the project:
```bash
mkdir -p build
cd build
cmake ..
make -j$(nproc)
```

3. Run the application:
```bash
./GraphicsProject
```

## Controls
- **WASD**: Move camera
- **Mouse**: Look around
- **Q/E**: Move up/down
- **ESC**: Exit

## Technical Details
- **OpenGL Version**: 3.3 Core Profile
- **Shaders**: GLSL 330
- **Libraries**: GLFW, GLAD, GLM, stb_image
- **Advanced Feature**: Instanced rendering for particles

## Project Structure
- `src/`: C++ source files
- `shaders/`: GLSL shader files
- `textures/`: Texture assets
- `external/`: Third-party dependencies

## License
Academic use only. See LICENSE for details.
