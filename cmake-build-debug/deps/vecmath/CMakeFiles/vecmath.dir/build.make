# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug"

# Include any dependencies generated for this target.
include deps/vecmath/CMakeFiles/vecmath.dir/depend.make

# Include the progress variables for this target.
include deps/vecmath/CMakeFiles/vecmath.dir/progress.make

# Include the compile flags for this target's objects.
include deps/vecmath/CMakeFiles/vecmath.dir/flags.make

deps/vecmath/CMakeFiles/vecmath.dir/src/Matrix2f.cpp.o: deps/vecmath/CMakeFiles/vecmath.dir/flags.make
deps/vecmath/CMakeFiles/vecmath.dir/src/Matrix2f.cpp.o: ../deps/vecmath/src/Matrix2f.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object deps/vecmath/CMakeFiles/vecmath.dir/src/Matrix2f.cpp.o"
	cd "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/deps/vecmath" && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/vecmath.dir/src/Matrix2f.cpp.o -c "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/deps/vecmath/src/Matrix2f.cpp"

deps/vecmath/CMakeFiles/vecmath.dir/src/Matrix2f.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vecmath.dir/src/Matrix2f.cpp.i"
	cd "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/deps/vecmath" && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/deps/vecmath/src/Matrix2f.cpp" > CMakeFiles/vecmath.dir/src/Matrix2f.cpp.i

deps/vecmath/CMakeFiles/vecmath.dir/src/Matrix2f.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vecmath.dir/src/Matrix2f.cpp.s"
	cd "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/deps/vecmath" && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/deps/vecmath/src/Matrix2f.cpp" -o CMakeFiles/vecmath.dir/src/Matrix2f.cpp.s

deps/vecmath/CMakeFiles/vecmath.dir/src/Matrix3f.cpp.o: deps/vecmath/CMakeFiles/vecmath.dir/flags.make
deps/vecmath/CMakeFiles/vecmath.dir/src/Matrix3f.cpp.o: ../deps/vecmath/src/Matrix3f.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object deps/vecmath/CMakeFiles/vecmath.dir/src/Matrix3f.cpp.o"
	cd "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/deps/vecmath" && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/vecmath.dir/src/Matrix3f.cpp.o -c "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/deps/vecmath/src/Matrix3f.cpp"

deps/vecmath/CMakeFiles/vecmath.dir/src/Matrix3f.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vecmath.dir/src/Matrix3f.cpp.i"
	cd "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/deps/vecmath" && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/deps/vecmath/src/Matrix3f.cpp" > CMakeFiles/vecmath.dir/src/Matrix3f.cpp.i

deps/vecmath/CMakeFiles/vecmath.dir/src/Matrix3f.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vecmath.dir/src/Matrix3f.cpp.s"
	cd "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/deps/vecmath" && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/deps/vecmath/src/Matrix3f.cpp" -o CMakeFiles/vecmath.dir/src/Matrix3f.cpp.s

deps/vecmath/CMakeFiles/vecmath.dir/src/Matrix4f.cpp.o: deps/vecmath/CMakeFiles/vecmath.dir/flags.make
deps/vecmath/CMakeFiles/vecmath.dir/src/Matrix4f.cpp.o: ../deps/vecmath/src/Matrix4f.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object deps/vecmath/CMakeFiles/vecmath.dir/src/Matrix4f.cpp.o"
	cd "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/deps/vecmath" && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/vecmath.dir/src/Matrix4f.cpp.o -c "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/deps/vecmath/src/Matrix4f.cpp"

deps/vecmath/CMakeFiles/vecmath.dir/src/Matrix4f.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vecmath.dir/src/Matrix4f.cpp.i"
	cd "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/deps/vecmath" && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/deps/vecmath/src/Matrix4f.cpp" > CMakeFiles/vecmath.dir/src/Matrix4f.cpp.i

deps/vecmath/CMakeFiles/vecmath.dir/src/Matrix4f.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vecmath.dir/src/Matrix4f.cpp.s"
	cd "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/deps/vecmath" && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/deps/vecmath/src/Matrix4f.cpp" -o CMakeFiles/vecmath.dir/src/Matrix4f.cpp.s

deps/vecmath/CMakeFiles/vecmath.dir/src/Quat4f.cpp.o: deps/vecmath/CMakeFiles/vecmath.dir/flags.make
deps/vecmath/CMakeFiles/vecmath.dir/src/Quat4f.cpp.o: ../deps/vecmath/src/Quat4f.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object deps/vecmath/CMakeFiles/vecmath.dir/src/Quat4f.cpp.o"
	cd "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/deps/vecmath" && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/vecmath.dir/src/Quat4f.cpp.o -c "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/deps/vecmath/src/Quat4f.cpp"

deps/vecmath/CMakeFiles/vecmath.dir/src/Quat4f.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vecmath.dir/src/Quat4f.cpp.i"
	cd "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/deps/vecmath" && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/deps/vecmath/src/Quat4f.cpp" > CMakeFiles/vecmath.dir/src/Quat4f.cpp.i

deps/vecmath/CMakeFiles/vecmath.dir/src/Quat4f.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vecmath.dir/src/Quat4f.cpp.s"
	cd "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/deps/vecmath" && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/deps/vecmath/src/Quat4f.cpp" -o CMakeFiles/vecmath.dir/src/Quat4f.cpp.s

deps/vecmath/CMakeFiles/vecmath.dir/src/Vector2f.cpp.o: deps/vecmath/CMakeFiles/vecmath.dir/flags.make
deps/vecmath/CMakeFiles/vecmath.dir/src/Vector2f.cpp.o: ../deps/vecmath/src/Vector2f.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object deps/vecmath/CMakeFiles/vecmath.dir/src/Vector2f.cpp.o"
	cd "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/deps/vecmath" && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/vecmath.dir/src/Vector2f.cpp.o -c "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/deps/vecmath/src/Vector2f.cpp"

deps/vecmath/CMakeFiles/vecmath.dir/src/Vector2f.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vecmath.dir/src/Vector2f.cpp.i"
	cd "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/deps/vecmath" && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/deps/vecmath/src/Vector2f.cpp" > CMakeFiles/vecmath.dir/src/Vector2f.cpp.i

deps/vecmath/CMakeFiles/vecmath.dir/src/Vector2f.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vecmath.dir/src/Vector2f.cpp.s"
	cd "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/deps/vecmath" && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/deps/vecmath/src/Vector2f.cpp" -o CMakeFiles/vecmath.dir/src/Vector2f.cpp.s

deps/vecmath/CMakeFiles/vecmath.dir/src/Vector3f.cpp.o: deps/vecmath/CMakeFiles/vecmath.dir/flags.make
deps/vecmath/CMakeFiles/vecmath.dir/src/Vector3f.cpp.o: ../deps/vecmath/src/Vector3f.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object deps/vecmath/CMakeFiles/vecmath.dir/src/Vector3f.cpp.o"
	cd "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/deps/vecmath" && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/vecmath.dir/src/Vector3f.cpp.o -c "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/deps/vecmath/src/Vector3f.cpp"

deps/vecmath/CMakeFiles/vecmath.dir/src/Vector3f.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vecmath.dir/src/Vector3f.cpp.i"
	cd "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/deps/vecmath" && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/deps/vecmath/src/Vector3f.cpp" > CMakeFiles/vecmath.dir/src/Vector3f.cpp.i

deps/vecmath/CMakeFiles/vecmath.dir/src/Vector3f.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vecmath.dir/src/Vector3f.cpp.s"
	cd "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/deps/vecmath" && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/deps/vecmath/src/Vector3f.cpp" -o CMakeFiles/vecmath.dir/src/Vector3f.cpp.s

deps/vecmath/CMakeFiles/vecmath.dir/src/Vector4f.cpp.o: deps/vecmath/CMakeFiles/vecmath.dir/flags.make
deps/vecmath/CMakeFiles/vecmath.dir/src/Vector4f.cpp.o: ../deps/vecmath/src/Vector4f.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object deps/vecmath/CMakeFiles/vecmath.dir/src/Vector4f.cpp.o"
	cd "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/deps/vecmath" && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/vecmath.dir/src/Vector4f.cpp.o -c "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/deps/vecmath/src/Vector4f.cpp"

deps/vecmath/CMakeFiles/vecmath.dir/src/Vector4f.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vecmath.dir/src/Vector4f.cpp.i"
	cd "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/deps/vecmath" && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/deps/vecmath/src/Vector4f.cpp" > CMakeFiles/vecmath.dir/src/Vector4f.cpp.i

deps/vecmath/CMakeFiles/vecmath.dir/src/Vector4f.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vecmath.dir/src/Vector4f.cpp.s"
	cd "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/deps/vecmath" && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/deps/vecmath/src/Vector4f.cpp" -o CMakeFiles/vecmath.dir/src/Vector4f.cpp.s

# Object files for target vecmath
vecmath_OBJECTS = \
"CMakeFiles/vecmath.dir/src/Matrix2f.cpp.o" \
"CMakeFiles/vecmath.dir/src/Matrix3f.cpp.o" \
"CMakeFiles/vecmath.dir/src/Matrix4f.cpp.o" \
"CMakeFiles/vecmath.dir/src/Quat4f.cpp.o" \
"CMakeFiles/vecmath.dir/src/Vector2f.cpp.o" \
"CMakeFiles/vecmath.dir/src/Vector3f.cpp.o" \
"CMakeFiles/vecmath.dir/src/Vector4f.cpp.o"

# External object files for target vecmath
vecmath_EXTERNAL_OBJECTS =

deps/vecmath/libvecmath.a: deps/vecmath/CMakeFiles/vecmath.dir/src/Matrix2f.cpp.o
deps/vecmath/libvecmath.a: deps/vecmath/CMakeFiles/vecmath.dir/src/Matrix3f.cpp.o
deps/vecmath/libvecmath.a: deps/vecmath/CMakeFiles/vecmath.dir/src/Matrix4f.cpp.o
deps/vecmath/libvecmath.a: deps/vecmath/CMakeFiles/vecmath.dir/src/Quat4f.cpp.o
deps/vecmath/libvecmath.a: deps/vecmath/CMakeFiles/vecmath.dir/src/Vector2f.cpp.o
deps/vecmath/libvecmath.a: deps/vecmath/CMakeFiles/vecmath.dir/src/Vector3f.cpp.o
deps/vecmath/libvecmath.a: deps/vecmath/CMakeFiles/vecmath.dir/src/Vector4f.cpp.o
deps/vecmath/libvecmath.a: deps/vecmath/CMakeFiles/vecmath.dir/build.make
deps/vecmath/libvecmath.a: deps/vecmath/CMakeFiles/vecmath.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX static library libvecmath.a"
	cd "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/deps/vecmath" && $(CMAKE_COMMAND) -P CMakeFiles/vecmath.dir/cmake_clean_target.cmake
	cd "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/deps/vecmath" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vecmath.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
deps/vecmath/CMakeFiles/vecmath.dir/build: deps/vecmath/libvecmath.a

.PHONY : deps/vecmath/CMakeFiles/vecmath.dir/build

deps/vecmath/CMakeFiles/vecmath.dir/clean:
	cd "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/deps/vecmath" && $(CMAKE_COMMAND) -P CMakeFiles/vecmath.dir/cmake_clean.cmake
.PHONY : deps/vecmath/CMakeFiles/vecmath.dir/clean

deps/vecmath/CMakeFiles/vecmath.dir/depend:
	cd "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer" "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/deps/vecmath" "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug" "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/deps/vecmath" "/Users/xrw/CLionProjects/Computational Geographic/ray_tracer/cmake-build-debug/deps/vecmath/CMakeFiles/vecmath.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : deps/vecmath/CMakeFiles/vecmath.dir/depend

