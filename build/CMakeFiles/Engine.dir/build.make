# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/pharaoht8/2D Engine"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/pharaoht8/2D Engine/build"

# Include any dependencies generated for this target.
include CMakeFiles/Engine.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Engine.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Engine.dir/flags.make

CMakeFiles/Engine.dir/src/imgui.cpp.o: CMakeFiles/Engine.dir/flags.make
CMakeFiles/Engine.dir/src/imgui.cpp.o: ../src/imgui.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/pharaoht8/2D Engine/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Engine.dir/src/imgui.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Engine.dir/src/imgui.cpp.o -c "/home/pharaoht8/2D Engine/src/imgui.cpp"

CMakeFiles/Engine.dir/src/imgui.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Engine.dir/src/imgui.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/pharaoht8/2D Engine/src/imgui.cpp" > CMakeFiles/Engine.dir/src/imgui.cpp.i

CMakeFiles/Engine.dir/src/imgui.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Engine.dir/src/imgui.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/pharaoht8/2D Engine/src/imgui.cpp" -o CMakeFiles/Engine.dir/src/imgui.cpp.s

CMakeFiles/Engine.dir/src/imgui_demo.cpp.o: CMakeFiles/Engine.dir/flags.make
CMakeFiles/Engine.dir/src/imgui_demo.cpp.o: ../src/imgui_demo.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/pharaoht8/2D Engine/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Engine.dir/src/imgui_demo.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Engine.dir/src/imgui_demo.cpp.o -c "/home/pharaoht8/2D Engine/src/imgui_demo.cpp"

CMakeFiles/Engine.dir/src/imgui_demo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Engine.dir/src/imgui_demo.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/pharaoht8/2D Engine/src/imgui_demo.cpp" > CMakeFiles/Engine.dir/src/imgui_demo.cpp.i

CMakeFiles/Engine.dir/src/imgui_demo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Engine.dir/src/imgui_demo.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/pharaoht8/2D Engine/src/imgui_demo.cpp" -o CMakeFiles/Engine.dir/src/imgui_demo.cpp.s

CMakeFiles/Engine.dir/src/imgui_draw.cpp.o: CMakeFiles/Engine.dir/flags.make
CMakeFiles/Engine.dir/src/imgui_draw.cpp.o: ../src/imgui_draw.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/pharaoht8/2D Engine/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Engine.dir/src/imgui_draw.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Engine.dir/src/imgui_draw.cpp.o -c "/home/pharaoht8/2D Engine/src/imgui_draw.cpp"

CMakeFiles/Engine.dir/src/imgui_draw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Engine.dir/src/imgui_draw.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/pharaoht8/2D Engine/src/imgui_draw.cpp" > CMakeFiles/Engine.dir/src/imgui_draw.cpp.i

CMakeFiles/Engine.dir/src/imgui_draw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Engine.dir/src/imgui_draw.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/pharaoht8/2D Engine/src/imgui_draw.cpp" -o CMakeFiles/Engine.dir/src/imgui_draw.cpp.s

CMakeFiles/Engine.dir/src/imgui_impl_glfw.cpp.o: CMakeFiles/Engine.dir/flags.make
CMakeFiles/Engine.dir/src/imgui_impl_glfw.cpp.o: ../src/imgui_impl_glfw.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/pharaoht8/2D Engine/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Engine.dir/src/imgui_impl_glfw.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Engine.dir/src/imgui_impl_glfw.cpp.o -c "/home/pharaoht8/2D Engine/src/imgui_impl_glfw.cpp"

CMakeFiles/Engine.dir/src/imgui_impl_glfw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Engine.dir/src/imgui_impl_glfw.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/pharaoht8/2D Engine/src/imgui_impl_glfw.cpp" > CMakeFiles/Engine.dir/src/imgui_impl_glfw.cpp.i

CMakeFiles/Engine.dir/src/imgui_impl_glfw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Engine.dir/src/imgui_impl_glfw.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/pharaoht8/2D Engine/src/imgui_impl_glfw.cpp" -o CMakeFiles/Engine.dir/src/imgui_impl_glfw.cpp.s

CMakeFiles/Engine.dir/src/imgui_impl_opengl3.cpp.o: CMakeFiles/Engine.dir/flags.make
CMakeFiles/Engine.dir/src/imgui_impl_opengl3.cpp.o: ../src/imgui_impl_opengl3.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/pharaoht8/2D Engine/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Engine.dir/src/imgui_impl_opengl3.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Engine.dir/src/imgui_impl_opengl3.cpp.o -c "/home/pharaoht8/2D Engine/src/imgui_impl_opengl3.cpp"

CMakeFiles/Engine.dir/src/imgui_impl_opengl3.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Engine.dir/src/imgui_impl_opengl3.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/pharaoht8/2D Engine/src/imgui_impl_opengl3.cpp" > CMakeFiles/Engine.dir/src/imgui_impl_opengl3.cpp.i

CMakeFiles/Engine.dir/src/imgui_impl_opengl3.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Engine.dir/src/imgui_impl_opengl3.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/pharaoht8/2D Engine/src/imgui_impl_opengl3.cpp" -o CMakeFiles/Engine.dir/src/imgui_impl_opengl3.cpp.s

CMakeFiles/Engine.dir/src/imgui_tables.cpp.o: CMakeFiles/Engine.dir/flags.make
CMakeFiles/Engine.dir/src/imgui_tables.cpp.o: ../src/imgui_tables.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/pharaoht8/2D Engine/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/Engine.dir/src/imgui_tables.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Engine.dir/src/imgui_tables.cpp.o -c "/home/pharaoht8/2D Engine/src/imgui_tables.cpp"

CMakeFiles/Engine.dir/src/imgui_tables.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Engine.dir/src/imgui_tables.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/pharaoht8/2D Engine/src/imgui_tables.cpp" > CMakeFiles/Engine.dir/src/imgui_tables.cpp.i

CMakeFiles/Engine.dir/src/imgui_tables.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Engine.dir/src/imgui_tables.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/pharaoht8/2D Engine/src/imgui_tables.cpp" -o CMakeFiles/Engine.dir/src/imgui_tables.cpp.s

CMakeFiles/Engine.dir/src/imgui_widgets.cpp.o: CMakeFiles/Engine.dir/flags.make
CMakeFiles/Engine.dir/src/imgui_widgets.cpp.o: ../src/imgui_widgets.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/pharaoht8/2D Engine/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/Engine.dir/src/imgui_widgets.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Engine.dir/src/imgui_widgets.cpp.o -c "/home/pharaoht8/2D Engine/src/imgui_widgets.cpp"

CMakeFiles/Engine.dir/src/imgui_widgets.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Engine.dir/src/imgui_widgets.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/pharaoht8/2D Engine/src/imgui_widgets.cpp" > CMakeFiles/Engine.dir/src/imgui_widgets.cpp.i

CMakeFiles/Engine.dir/src/imgui_widgets.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Engine.dir/src/imgui_widgets.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/pharaoht8/2D Engine/src/imgui_widgets.cpp" -o CMakeFiles/Engine.dir/src/imgui_widgets.cpp.s

CMakeFiles/Engine.dir/src/main.cpp.o: CMakeFiles/Engine.dir/flags.make
CMakeFiles/Engine.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/pharaoht8/2D Engine/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/Engine.dir/src/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Engine.dir/src/main.cpp.o -c "/home/pharaoht8/2D Engine/src/main.cpp"

CMakeFiles/Engine.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Engine.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/pharaoht8/2D Engine/src/main.cpp" > CMakeFiles/Engine.dir/src/main.cpp.i

CMakeFiles/Engine.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Engine.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/pharaoht8/2D Engine/src/main.cpp" -o CMakeFiles/Engine.dir/src/main.cpp.s

CMakeFiles/Engine.dir/src/stb_image.cpp.o: CMakeFiles/Engine.dir/flags.make
CMakeFiles/Engine.dir/src/stb_image.cpp.o: ../src/stb_image.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/pharaoht8/2D Engine/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/Engine.dir/src/stb_image.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Engine.dir/src/stb_image.cpp.o -c "/home/pharaoht8/2D Engine/src/stb_image.cpp"

CMakeFiles/Engine.dir/src/stb_image.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Engine.dir/src/stb_image.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/pharaoht8/2D Engine/src/stb_image.cpp" > CMakeFiles/Engine.dir/src/stb_image.cpp.i

CMakeFiles/Engine.dir/src/stb_image.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Engine.dir/src/stb_image.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/pharaoht8/2D Engine/src/stb_image.cpp" -o CMakeFiles/Engine.dir/src/stb_image.cpp.s

# Object files for target Engine
Engine_OBJECTS = \
"CMakeFiles/Engine.dir/src/imgui.cpp.o" \
"CMakeFiles/Engine.dir/src/imgui_demo.cpp.o" \
"CMakeFiles/Engine.dir/src/imgui_draw.cpp.o" \
"CMakeFiles/Engine.dir/src/imgui_impl_glfw.cpp.o" \
"CMakeFiles/Engine.dir/src/imgui_impl_opengl3.cpp.o" \
"CMakeFiles/Engine.dir/src/imgui_tables.cpp.o" \
"CMakeFiles/Engine.dir/src/imgui_widgets.cpp.o" \
"CMakeFiles/Engine.dir/src/main.cpp.o" \
"CMakeFiles/Engine.dir/src/stb_image.cpp.o"

# External object files for target Engine
Engine_EXTERNAL_OBJECTS =

Engine: CMakeFiles/Engine.dir/src/imgui.cpp.o
Engine: CMakeFiles/Engine.dir/src/imgui_demo.cpp.o
Engine: CMakeFiles/Engine.dir/src/imgui_draw.cpp.o
Engine: CMakeFiles/Engine.dir/src/imgui_impl_glfw.cpp.o
Engine: CMakeFiles/Engine.dir/src/imgui_impl_opengl3.cpp.o
Engine: CMakeFiles/Engine.dir/src/imgui_tables.cpp.o
Engine: CMakeFiles/Engine.dir/src/imgui_widgets.cpp.o
Engine: CMakeFiles/Engine.dir/src/main.cpp.o
Engine: CMakeFiles/Engine.dir/src/stb_image.cpp.o
Engine: CMakeFiles/Engine.dir/build.make
Engine: glfw-3.3.4/src/libglfw3.a
Engine: ../glew-2.1.0/lib/libGLEW.a
Engine: /usr/lib/x86_64-linux-gnu/librt.so
Engine: /usr/lib/x86_64-linux-gnu/libm.so
Engine: /usr/lib/x86_64-linux-gnu/libX11.so
Engine: /usr/lib/x86_64-linux-gnu/libGLX.so
Engine: /usr/lib/x86_64-linux-gnu/libOpenGL.so
Engine: CMakeFiles/Engine.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/pharaoht8/2D Engine/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX executable Engine"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Engine.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Engine.dir/build: Engine

.PHONY : CMakeFiles/Engine.dir/build

CMakeFiles/Engine.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Engine.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Engine.dir/clean

CMakeFiles/Engine.dir/depend:
	cd "/home/pharaoht8/2D Engine/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/pharaoht8/2D Engine" "/home/pharaoht8/2D Engine" "/home/pharaoht8/2D Engine/build" "/home/pharaoht8/2D Engine/build" "/home/pharaoht8/2D Engine/build/CMakeFiles/Engine.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/Engine.dir/depend

