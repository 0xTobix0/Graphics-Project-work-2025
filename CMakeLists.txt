cmake_minimum_required(VERSION 3.10)

project(GraphicsProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set C++17 flags only for C++ files
if(MSVC)
    add_compile_options(
        $<$<COMPILE_LANGUAGE:CXX>:/std:c++17>
    )
else()
    add_compile_options(
        $<$<COMPILE_LANGUAGE:CXX>:-std=c++17>
    )
endif()

# Add GLFW
add_subdirectory(external/glfw)

# Find required packages
find_package(OpenGL REQUIRED)

# Enable C++11 for tinygltf
set(CMAKE_CXX_STANDARD 11)  # tinygltf requires C++11
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/external/glfw/include
    ${CMAKE_SOURCE_DIR}/external/glad-3.3/include
    ${CMAKE_SOURCE_DIR}/external/glm-0.9.7.1
    ${CMAKE_SOURCE_DIR}/external/tinygltf
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glm-0.9.7.1/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/external
    ${CMAKE_CURRENT_SOURCE_DIR}/external/tinygltf
    ${CMAKE_CURRENT_SOURCE_DIR}/external/stb
    ${CMAKE_CURRENT_SOURCE_DIR}/external/tinygltf/tiny_gltf.h
)

# Add tinygltf source files (kept for potential future use)
set(TINYGLTF_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/external/tinygltf/tiny_gltf.cc
)

# Create a library for tinygltf (kept for potential future use)
add_library(tinygltf STATIC ${TINYGLTF_SOURCES})
target_include_directories(tinygltf PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/external/tinygltf
    ${CMAKE_CURRENT_SOURCE_DIR}/external/stb
)

# Add executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/skybox.cpp
    src/shader_manager.cpp
    src/butterfly.cpp
    src/obj_loader.cpp
    external/glad-3.3/src/gl.c
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARIES}
    glfw
    ${CMAKE_DL_LIBS}
    m
    tinygltf
)

# Copy shaders to build directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/shaders)
file(GLOB SHADER_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.vert" 
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.frag"
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/butterfly.*"
)
foreach(SHADER ${SHADER_FILES})
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} ${CMAKE_BINARY_DIR}/shaders/
        DEPENDS ${SHADER}
        COMMENT "Copying shader: ${SHADER}"
    )
endforeach()

# Copy skybox cubemap textures to build directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/textures/skybox_cubemap)
file(GLOB SKYBOX_FILES "${PROJECT_SOURCE_DIR}/textures/skybox_cubemap/*.png")
foreach(SKYBOX_FILE ${SKYBOX_FILES})
    file(COPY ${SKYBOX_FILE} DESTINATION ${CMAKE_BINARY_DIR}/textures/skybox_cubemap)
    message(STATUS "Copying skybox cubemap texture: ${SKYBOX_FILE}")
endforeach()

# Set working directory to the executable directory so shaders can be found
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
