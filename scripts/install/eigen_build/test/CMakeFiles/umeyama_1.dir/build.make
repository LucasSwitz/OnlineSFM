# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/gumbo00/OnlineSFM/scripts/install/eigen

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/gumbo00/OnlineSFM/scripts/install/eigen_build

# Include any dependencies generated for this target.
include test/CMakeFiles/umeyama_1.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/umeyama_1.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/umeyama_1.dir/flags.make

test/CMakeFiles/umeyama_1.dir/umeyama.cpp.o: test/CMakeFiles/umeyama_1.dir/flags.make
test/CMakeFiles/umeyama_1.dir/umeyama.cpp.o: /home/gumbo00/OnlineSFM/scripts/install/eigen/test/umeyama.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gumbo00/OnlineSFM/scripts/install/eigen_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/umeyama_1.dir/umeyama.cpp.o"
	cd /home/gumbo00/OnlineSFM/scripts/install/eigen_build/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/umeyama_1.dir/umeyama.cpp.o -c /home/gumbo00/OnlineSFM/scripts/install/eigen/test/umeyama.cpp

test/CMakeFiles/umeyama_1.dir/umeyama.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/umeyama_1.dir/umeyama.cpp.i"
	cd /home/gumbo00/OnlineSFM/scripts/install/eigen_build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gumbo00/OnlineSFM/scripts/install/eigen/test/umeyama.cpp > CMakeFiles/umeyama_1.dir/umeyama.cpp.i

test/CMakeFiles/umeyama_1.dir/umeyama.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/umeyama_1.dir/umeyama.cpp.s"
	cd /home/gumbo00/OnlineSFM/scripts/install/eigen_build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gumbo00/OnlineSFM/scripts/install/eigen/test/umeyama.cpp -o CMakeFiles/umeyama_1.dir/umeyama.cpp.s

# Object files for target umeyama_1
umeyama_1_OBJECTS = \
"CMakeFiles/umeyama_1.dir/umeyama.cpp.o"

# External object files for target umeyama_1
umeyama_1_EXTERNAL_OBJECTS =

test/umeyama_1: test/CMakeFiles/umeyama_1.dir/umeyama.cpp.o
test/umeyama_1: test/CMakeFiles/umeyama_1.dir/build.make
test/umeyama_1: test/CMakeFiles/umeyama_1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/gumbo00/OnlineSFM/scripts/install/eigen_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable umeyama_1"
	cd /home/gumbo00/OnlineSFM/scripts/install/eigen_build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/umeyama_1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/umeyama_1.dir/build: test/umeyama_1

.PHONY : test/CMakeFiles/umeyama_1.dir/build

test/CMakeFiles/umeyama_1.dir/clean:
	cd /home/gumbo00/OnlineSFM/scripts/install/eigen_build/test && $(CMAKE_COMMAND) -P CMakeFiles/umeyama_1.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/umeyama_1.dir/clean

test/CMakeFiles/umeyama_1.dir/depend:
	cd /home/gumbo00/OnlineSFM/scripts/install/eigen_build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gumbo00/OnlineSFM/scripts/install/eigen /home/gumbo00/OnlineSFM/scripts/install/eigen/test /home/gumbo00/OnlineSFM/scripts/install/eigen_build /home/gumbo00/OnlineSFM/scripts/install/eigen_build/test /home/gumbo00/OnlineSFM/scripts/install/eigen_build/test/CMakeFiles/umeyama_1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/umeyama_1.dir/depend

