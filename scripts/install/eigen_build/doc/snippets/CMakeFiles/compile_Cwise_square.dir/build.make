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
include doc/snippets/CMakeFiles/compile_Cwise_square.dir/depend.make

# Include the progress variables for this target.
include doc/snippets/CMakeFiles/compile_Cwise_square.dir/progress.make

# Include the compile flags for this target's objects.
include doc/snippets/CMakeFiles/compile_Cwise_square.dir/flags.make

doc/snippets/CMakeFiles/compile_Cwise_square.dir/compile_Cwise_square.cpp.o: doc/snippets/CMakeFiles/compile_Cwise_square.dir/flags.make
doc/snippets/CMakeFiles/compile_Cwise_square.dir/compile_Cwise_square.cpp.o: doc/snippets/compile_Cwise_square.cpp
doc/snippets/CMakeFiles/compile_Cwise_square.dir/compile_Cwise_square.cpp.o: /home/gumbo00/OnlineSFM/scripts/install/eigen/doc/snippets/Cwise_square.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gumbo00/OnlineSFM/scripts/install/eigen_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object doc/snippets/CMakeFiles/compile_Cwise_square.dir/compile_Cwise_square.cpp.o"
	cd /home/gumbo00/OnlineSFM/scripts/install/eigen_build/doc/snippets && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/compile_Cwise_square.dir/compile_Cwise_square.cpp.o -c /home/gumbo00/OnlineSFM/scripts/install/eigen_build/doc/snippets/compile_Cwise_square.cpp

doc/snippets/CMakeFiles/compile_Cwise_square.dir/compile_Cwise_square.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/compile_Cwise_square.dir/compile_Cwise_square.cpp.i"
	cd /home/gumbo00/OnlineSFM/scripts/install/eigen_build/doc/snippets && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gumbo00/OnlineSFM/scripts/install/eigen_build/doc/snippets/compile_Cwise_square.cpp > CMakeFiles/compile_Cwise_square.dir/compile_Cwise_square.cpp.i

doc/snippets/CMakeFiles/compile_Cwise_square.dir/compile_Cwise_square.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/compile_Cwise_square.dir/compile_Cwise_square.cpp.s"
	cd /home/gumbo00/OnlineSFM/scripts/install/eigen_build/doc/snippets && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gumbo00/OnlineSFM/scripts/install/eigen_build/doc/snippets/compile_Cwise_square.cpp -o CMakeFiles/compile_Cwise_square.dir/compile_Cwise_square.cpp.s

# Object files for target compile_Cwise_square
compile_Cwise_square_OBJECTS = \
"CMakeFiles/compile_Cwise_square.dir/compile_Cwise_square.cpp.o"

# External object files for target compile_Cwise_square
compile_Cwise_square_EXTERNAL_OBJECTS =

doc/snippets/compile_Cwise_square: doc/snippets/CMakeFiles/compile_Cwise_square.dir/compile_Cwise_square.cpp.o
doc/snippets/compile_Cwise_square: doc/snippets/CMakeFiles/compile_Cwise_square.dir/build.make
doc/snippets/compile_Cwise_square: doc/snippets/CMakeFiles/compile_Cwise_square.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/gumbo00/OnlineSFM/scripts/install/eigen_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable compile_Cwise_square"
	cd /home/gumbo00/OnlineSFM/scripts/install/eigen_build/doc/snippets && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/compile_Cwise_square.dir/link.txt --verbose=$(VERBOSE)
	cd /home/gumbo00/OnlineSFM/scripts/install/eigen_build/doc/snippets && ./compile_Cwise_square >/home/gumbo00/OnlineSFM/scripts/install/eigen_build/doc/snippets/Cwise_square.out

# Rule to build all files generated by this target.
doc/snippets/CMakeFiles/compile_Cwise_square.dir/build: doc/snippets/compile_Cwise_square

.PHONY : doc/snippets/CMakeFiles/compile_Cwise_square.dir/build

doc/snippets/CMakeFiles/compile_Cwise_square.dir/clean:
	cd /home/gumbo00/OnlineSFM/scripts/install/eigen_build/doc/snippets && $(CMAKE_COMMAND) -P CMakeFiles/compile_Cwise_square.dir/cmake_clean.cmake
.PHONY : doc/snippets/CMakeFiles/compile_Cwise_square.dir/clean

doc/snippets/CMakeFiles/compile_Cwise_square.dir/depend:
	cd /home/gumbo00/OnlineSFM/scripts/install/eigen_build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gumbo00/OnlineSFM/scripts/install/eigen /home/gumbo00/OnlineSFM/scripts/install/eigen/doc/snippets /home/gumbo00/OnlineSFM/scripts/install/eigen_build /home/gumbo00/OnlineSFM/scripts/install/eigen_build/doc/snippets /home/gumbo00/OnlineSFM/scripts/install/eigen_build/doc/snippets/CMakeFiles/compile_Cwise_square.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : doc/snippets/CMakeFiles/compile_Cwise_square.dir/depend

