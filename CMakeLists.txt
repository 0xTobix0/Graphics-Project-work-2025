cmake_minimum_required(VERSION 3.10)

project(GraphicsProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLEW REQUIRED)

# Add executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/Camera.cpp
    src/Shader.cpp
    src/Model.cpp
)

# Include directories
include_directories(src)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARIES}
    ${GLFW3_LIBRARIES}
    ${GLEW_LIBRARIES}
)
