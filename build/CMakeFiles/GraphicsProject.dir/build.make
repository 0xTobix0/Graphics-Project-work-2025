# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/namangupta/CascadeProjects/graphics-project-work-2025

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/namangupta/CascadeProjects/graphics-project-work-2025/build

# Include any dependencies generated for this target.
include CMakeFiles/GraphicsProject.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/GraphicsProject.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/GraphicsProject.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/GraphicsProject.dir/flags.make

CMakeFiles/GraphicsProject.dir/codegen:
.PHONY : CMakeFiles/GraphicsProject.dir/codegen

CMakeFiles/GraphicsProject.dir/src/main.cpp.o: CMakeFiles/GraphicsProject.dir/flags.make
CMakeFiles/GraphicsProject.dir/src/main.cpp.o: /Users/namangupta/CascadeProjects/graphics-project-work-2025/src/main.cpp
CMakeFiles/GraphicsProject.dir/src/main.cpp.o: CMakeFiles/GraphicsProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/namangupta/CascadeProjects/graphics-project-work-2025/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/GraphicsProject.dir/src/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/GraphicsProject.dir/src/main.cpp.o -MF CMakeFiles/GraphicsProject.dir/src/main.cpp.o.d -o CMakeFiles/GraphicsProject.dir/src/main.cpp.o -c /Users/namangupta/CascadeProjects/graphics-project-work-2025/src/main.cpp

CMakeFiles/GraphicsProject.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/GraphicsProject.dir/src/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/namangupta/CascadeProjects/graphics-project-work-2025/src/main.cpp > CMakeFiles/GraphicsProject.dir/src/main.cpp.i

CMakeFiles/GraphicsProject.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/GraphicsProject.dir/src/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/namangupta/CascadeProjects/graphics-project-work-2025/src/main.cpp -o CMakeFiles/GraphicsProject.dir/src/main.cpp.s

CMakeFiles/GraphicsProject.dir/src/shader_manager.cpp.o: CMakeFiles/GraphicsProject.dir/flags.make
CMakeFiles/GraphicsProject.dir/src/shader_manager.cpp.o: /Users/namangupta/CascadeProjects/graphics-project-work-2025/src/shader_manager.cpp
CMakeFiles/GraphicsProject.dir/src/shader_manager.cpp.o: CMakeFiles/GraphicsProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/namangupta/CascadeProjects/graphics-project-work-2025/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/GraphicsProject.dir/src/shader_manager.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/GraphicsProject.dir/src/shader_manager.cpp.o -MF CMakeFiles/GraphicsProject.dir/src/shader_manager.cpp.o.d -o CMakeFiles/GraphicsProject.dir/src/shader_manager.cpp.o -c /Users/namangupta/CascadeProjects/graphics-project-work-2025/src/shader_manager.cpp

CMakeFiles/GraphicsProject.dir/src/shader_manager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/GraphicsProject.dir/src/shader_manager.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/namangupta/CascadeProjects/graphics-project-work-2025/src/shader_manager.cpp > CMakeFiles/GraphicsProject.dir/src/shader_manager.cpp.i

CMakeFiles/GraphicsProject.dir/src/shader_manager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/GraphicsProject.dir/src/shader_manager.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/namangupta/CascadeProjects/graphics-project-work-2025/src/shader_manager.cpp -o CMakeFiles/GraphicsProject.dir/src/shader_manager.cpp.s

CMakeFiles/GraphicsProject.dir/src/skybox.cpp.o: CMakeFiles/GraphicsProject.dir/flags.make
CMakeFiles/GraphicsProject.dir/src/skybox.cpp.o: /Users/namangupta/CascadeProjects/graphics-project-work-2025/src/skybox.cpp
CMakeFiles/GraphicsProject.dir/src/skybox.cpp.o: CMakeFiles/GraphicsProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/namangupta/CascadeProjects/graphics-project-work-2025/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/GraphicsProject.dir/src/skybox.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/GraphicsProject.dir/src/skybox.cpp.o -MF CMakeFiles/GraphicsProject.dir/src/skybox.cpp.o.d -o CMakeFiles/GraphicsProject.dir/src/skybox.cpp.o -c /Users/namangupta/CascadeProjects/graphics-project-work-2025/src/skybox.cpp

CMakeFiles/GraphicsProject.dir/src/skybox.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/GraphicsProject.dir/src/skybox.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/namangupta/CascadeProjects/graphics-project-work-2025/src/skybox.cpp > CMakeFiles/GraphicsProject.dir/src/skybox.cpp.i

CMakeFiles/GraphicsProject.dir/src/skybox.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/GraphicsProject.dir/src/skybox.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/namangupta/CascadeProjects/graphics-project-work-2025/src/skybox.cpp -o CMakeFiles/GraphicsProject.dir/src/skybox.cpp.s

CMakeFiles/GraphicsProject.dir/src/butterfly.cpp.o: CMakeFiles/GraphicsProject.dir/flags.make
CMakeFiles/GraphicsProject.dir/src/butterfly.cpp.o: /Users/namangupta/CascadeProjects/graphics-project-work-2025/src/butterfly.cpp
CMakeFiles/GraphicsProject.dir/src/butterfly.cpp.o: CMakeFiles/GraphicsProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/namangupta/CascadeProjects/graphics-project-work-2025/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/GraphicsProject.dir/src/butterfly.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/GraphicsProject.dir/src/butterfly.cpp.o -MF CMakeFiles/GraphicsProject.dir/src/butterfly.cpp.o.d -o CMakeFiles/GraphicsProject.dir/src/butterfly.cpp.o -c /Users/namangupta/CascadeProjects/graphics-project-work-2025/src/butterfly.cpp

CMakeFiles/GraphicsProject.dir/src/butterfly.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/GraphicsProject.dir/src/butterfly.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/namangupta/CascadeProjects/graphics-project-work-2025/src/butterfly.cpp > CMakeFiles/GraphicsProject.dir/src/butterfly.cpp.i

CMakeFiles/GraphicsProject.dir/src/butterfly.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/GraphicsProject.dir/src/butterfly.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/namangupta/CascadeProjects/graphics-project-work-2025/src/butterfly.cpp -o CMakeFiles/GraphicsProject.dir/src/butterfly.cpp.s

CMakeFiles/GraphicsProject.dir/src/obj_loader.cpp.o: CMakeFiles/GraphicsProject.dir/flags.make
CMakeFiles/GraphicsProject.dir/src/obj_loader.cpp.o: /Users/namangupta/CascadeProjects/graphics-project-work-2025/src/obj_loader.cpp
CMakeFiles/GraphicsProject.dir/src/obj_loader.cpp.o: CMakeFiles/GraphicsProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/namangupta/CascadeProjects/graphics-project-work-2025/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/GraphicsProject.dir/src/obj_loader.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/GraphicsProject.dir/src/obj_loader.cpp.o -MF CMakeFiles/GraphicsProject.dir/src/obj_loader.cpp.o.d -o CMakeFiles/GraphicsProject.dir/src/obj_loader.cpp.o -c /Users/namangupta/CascadeProjects/graphics-project-work-2025/src/obj_loader.cpp

CMakeFiles/GraphicsProject.dir/src/obj_loader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/GraphicsProject.dir/src/obj_loader.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/namangupta/CascadeProjects/graphics-project-work-2025/src/obj_loader.cpp > CMakeFiles/GraphicsProject.dir/src/obj_loader.cpp.i

CMakeFiles/GraphicsProject.dir/src/obj_loader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/GraphicsProject.dir/src/obj_loader.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/namangupta/CascadeProjects/graphics-project-work-2025/src/obj_loader.cpp -o CMakeFiles/GraphicsProject.dir/src/obj_loader.cpp.s

CMakeFiles/GraphicsProject.dir/src/text_renderer.cpp.o: CMakeFiles/GraphicsProject.dir/flags.make
CMakeFiles/GraphicsProject.dir/src/text_renderer.cpp.o: /Users/namangupta/CascadeProjects/graphics-project-work-2025/src/text_renderer.cpp
CMakeFiles/GraphicsProject.dir/src/text_renderer.cpp.o: CMakeFiles/GraphicsProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/namangupta/CascadeProjects/graphics-project-work-2025/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/GraphicsProject.dir/src/text_renderer.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/GraphicsProject.dir/src/text_renderer.cpp.o -MF CMakeFiles/GraphicsProject.dir/src/text_renderer.cpp.o.d -o CMakeFiles/GraphicsProject.dir/src/text_renderer.cpp.o -c /Users/namangupta/CascadeProjects/graphics-project-work-2025/src/text_renderer.cpp

CMakeFiles/GraphicsProject.dir/src/text_renderer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/GraphicsProject.dir/src/text_renderer.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/namangupta/CascadeProjects/graphics-project-work-2025/src/text_renderer.cpp > CMakeFiles/GraphicsProject.dir/src/text_renderer.cpp.i

CMakeFiles/GraphicsProject.dir/src/text_renderer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/GraphicsProject.dir/src/text_renderer.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/namangupta/CascadeProjects/graphics-project-work-2025/src/text_renderer.cpp -o CMakeFiles/GraphicsProject.dir/src/text_renderer.cpp.s

CMakeFiles/GraphicsProject.dir/external/tinygltf/tiny_gltf.cc.o: CMakeFiles/GraphicsProject.dir/flags.make
CMakeFiles/GraphicsProject.dir/external/tinygltf/tiny_gltf.cc.o: /Users/namangupta/CascadeProjects/graphics-project-work-2025/external/tinygltf/tiny_gltf.cc
CMakeFiles/GraphicsProject.dir/external/tinygltf/tiny_gltf.cc.o: CMakeFiles/GraphicsProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/namangupta/CascadeProjects/graphics-project-work-2025/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/GraphicsProject.dir/external/tinygltf/tiny_gltf.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/GraphicsProject.dir/external/tinygltf/tiny_gltf.cc.o -MF CMakeFiles/GraphicsProject.dir/external/tinygltf/tiny_gltf.cc.o.d -o CMakeFiles/GraphicsProject.dir/external/tinygltf/tiny_gltf.cc.o -c /Users/namangupta/CascadeProjects/graphics-project-work-2025/external/tinygltf/tiny_gltf.cc

CMakeFiles/GraphicsProject.dir/external/tinygltf/tiny_gltf.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/GraphicsProject.dir/external/tinygltf/tiny_gltf.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/namangupta/CascadeProjects/graphics-project-work-2025/external/tinygltf/tiny_gltf.cc > CMakeFiles/GraphicsProject.dir/external/tinygltf/tiny_gltf.cc.i

CMakeFiles/GraphicsProject.dir/external/tinygltf/tiny_gltf.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/GraphicsProject.dir/external/tinygltf/tiny_gltf.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/namangupta/CascadeProjects/graphics-project-work-2025/external/tinygltf/tiny_gltf.cc -o CMakeFiles/GraphicsProject.dir/external/tinygltf/tiny_gltf.cc.s

# Object files for target GraphicsProject
GraphicsProject_OBJECTS = \
"CMakeFiles/GraphicsProject.dir/src/main.cpp.o" \
"CMakeFiles/GraphicsProject.dir/src/shader_manager.cpp.o" \
"CMakeFiles/GraphicsProject.dir/src/skybox.cpp.o" \
"CMakeFiles/GraphicsProject.dir/src/butterfly.cpp.o" \
"CMakeFiles/GraphicsProject.dir/src/obj_loader.cpp.o" \
"CMakeFiles/GraphicsProject.dir/src/text_renderer.cpp.o" \
"CMakeFiles/GraphicsProject.dir/external/tinygltf/tiny_gltf.cc.o"

# External object files for target GraphicsProject
GraphicsProject_EXTERNAL_OBJECTS =

GraphicsProject: CMakeFiles/GraphicsProject.dir/src/main.cpp.o
GraphicsProject: CMakeFiles/GraphicsProject.dir/src/shader_manager.cpp.o
GraphicsProject: CMakeFiles/GraphicsProject.dir/src/skybox.cpp.o
GraphicsProject: CMakeFiles/GraphicsProject.dir/src/butterfly.cpp.o
GraphicsProject: CMakeFiles/GraphicsProject.dir/src/obj_loader.cpp.o
GraphicsProject: CMakeFiles/GraphicsProject.dir/src/text_renderer.cpp.o
GraphicsProject: CMakeFiles/GraphicsProject.dir/external/tinygltf/tiny_gltf.cc.o
GraphicsProject: CMakeFiles/GraphicsProject.dir/build.make
GraphicsProject: external/glfw/src/libglfw3.a
GraphicsProject: libglad.a
GraphicsProject: /opt/homebrew/lib/libfreetype.dylib
GraphicsProject: libtinygltf.a
GraphicsProject: CMakeFiles/GraphicsProject.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/namangupta/CascadeProjects/graphics-project-work-2025/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable GraphicsProject"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/GraphicsProject.dir/link.txt --verbose=$(VERBOSE)
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold "Copying shader: /Users/namangupta/CascadeProjects/graphics-project-work-2025/shaders/butterfly.vert"
	/opt/homebrew/bin/cmake -E copy /Users/namangupta/CascadeProjects/graphics-project-work-2025/shaders/butterfly.vert /Users/namangupta/CascadeProjects/graphics-project-work-2025/build/shaders/
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold "Copying shader: /Users/namangupta/CascadeProjects/graphics-project-work-2025/shaders/butterfly.frag"
	/opt/homebrew/bin/cmake -E copy /Users/namangupta/CascadeProjects/graphics-project-work-2025/shaders/butterfly.frag /Users/namangupta/CascadeProjects/graphics-project-work-2025/build/shaders/
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold "Copying shader: /Users/namangupta/CascadeProjects/graphics-project-work-2025/shaders/skybox.vert"
	/opt/homebrew/bin/cmake -E copy /Users/namangupta/CascadeProjects/graphics-project-work-2025/shaders/skybox.vert /Users/namangupta/CascadeProjects/graphics-project-work-2025/build/shaders/
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold "Copying shader: /Users/namangupta/CascadeProjects/graphics-project-work-2025/shaders/skybox.frag"
	/opt/homebrew/bin/cmake -E copy /Users/namangupta/CascadeProjects/graphics-project-work-2025/shaders/skybox.frag /Users/namangupta/CascadeProjects/graphics-project-work-2025/build/shaders/
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold "Copying shader: /Users/namangupta/CascadeProjects/graphics-project-work-2025/shaders/text.vert"
	/opt/homebrew/bin/cmake -E copy /Users/namangupta/CascadeProjects/graphics-project-work-2025/shaders/text.vert /Users/namangupta/CascadeProjects/graphics-project-work-2025/build/shaders/
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold "Copying shader: /Users/namangupta/CascadeProjects/graphics-project-work-2025/shaders/text.frag"
	/opt/homebrew/bin/cmake -E copy /Users/namangupta/CascadeProjects/graphics-project-work-2025/shaders/text.frag /Users/namangupta/CascadeProjects/graphics-project-work-2025/build/shaders/

# Rule to build all files generated by this target.
CMakeFiles/GraphicsProject.dir/build: GraphicsProject
.PHONY : CMakeFiles/GraphicsProject.dir/build

CMakeFiles/GraphicsProject.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/GraphicsProject.dir/cmake_clean.cmake
.PHONY : CMakeFiles/GraphicsProject.dir/clean

CMakeFiles/GraphicsProject.dir/depend:
	cd /Users/namangupta/CascadeProjects/graphics-project-work-2025/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/namangupta/CascadeProjects/graphics-project-work-2025 /Users/namangupta/CascadeProjects/graphics-project-work-2025 /Users/namangupta/CascadeProjects/graphics-project-work-2025/build /Users/namangupta/CascadeProjects/graphics-project-work-2025/build /Users/namangupta/CascadeProjects/graphics-project-work-2025/build/CMakeFiles/GraphicsProject.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/GraphicsProject.dir/depend

