cmake_minimum_required(VERSION 3.10)

project(GraphicsProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include external libraries
add_subdirectory(external)

# Find required packages
find_package(OpenGL REQUIRED)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw-3.1.2/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad-3.3/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glm-0.9.7.1
    ${CMAKE_CURRENT_SOURCE_DIR}/external/stb
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad-3.3/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glm-0.9.7.1/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/external
)

# Add executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/skybox.cpp
    external/glad-3.3/src/gl.c
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARIES}
    glfw
    ${CMAKE_DL_LIBS}
)

# Copy shaders to build directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/shaders)
file(GLOB SHADER_FILES "${PROJECT_SOURCE_DIR}/shaders/*")
foreach(SHADER_FILE ${SHADER_FILES})
    file(COPY ${SHADER_FILE} DESTINATION ${CMAKE_BINARY_DIR}/shaders)
    message(STATUS "Copying shader: ${SHADER_FILE}")
endforeach()

# Copy textures to build directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/textures/skybox)
file(GLOB SKYBOX_TEXTURES "${PROJECT_SOURCE_DIR}/textures/skybox/*")
foreach(TEXTURE_FILE ${SKYBOX_TEXTURES})
    file(COPY ${TEXTURE_FILE} DESTINATION ${CMAKE_BINARY_DIR}/textures/skybox)
    message(STATUS "Copying texture: ${TEXTURE_FILE}")
endforeach()

# Set working directory to the executable directory so shaders can be found
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
