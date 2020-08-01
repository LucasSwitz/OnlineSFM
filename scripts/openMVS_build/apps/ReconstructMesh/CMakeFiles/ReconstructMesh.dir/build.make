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
CMAKE_SOURCE_DIR = /home/gumbo00/OnlineSFM/scripts/install/openMVS

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/gumbo00/OnlineSFM/scripts/install/openMVS_build

# Include any dependencies generated for this target.
include apps/ReconstructMesh/CMakeFiles/ReconstructMesh.dir/depend.make

# Include the progress variables for this target.
include apps/ReconstructMesh/CMakeFiles/ReconstructMesh.dir/progress.make

# Include the compile flags for this target's objects.
include apps/ReconstructMesh/CMakeFiles/ReconstructMesh.dir/flags.make

apps/ReconstructMesh/CMakeFiles/ReconstructMesh.dir/ReconstructMesh.cpp.o: apps/ReconstructMesh/CMakeFiles/ReconstructMesh.dir/flags.make
apps/ReconstructMesh/CMakeFiles/ReconstructMesh.dir/ReconstructMesh.cpp.o: /home/gumbo00/OnlineSFM/scripts/install/openMVS/apps/ReconstructMesh/ReconstructMesh.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gumbo00/OnlineSFM/scripts/install/openMVS_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object apps/ReconstructMesh/CMakeFiles/ReconstructMesh.dir/ReconstructMesh.cpp.o"
	cd /home/gumbo00/OnlineSFM/scripts/install/openMVS_build/apps/ReconstructMesh && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ReconstructMesh.dir/ReconstructMesh.cpp.o -c /home/gumbo00/OnlineSFM/scripts/install/openMVS/apps/ReconstructMesh/ReconstructMesh.cpp

apps/ReconstructMesh/CMakeFiles/ReconstructMesh.dir/ReconstructMesh.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ReconstructMesh.dir/ReconstructMesh.cpp.i"
	cd /home/gumbo00/OnlineSFM/scripts/install/openMVS_build/apps/ReconstructMesh && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gumbo00/OnlineSFM/scripts/install/openMVS/apps/ReconstructMesh/ReconstructMesh.cpp > CMakeFiles/ReconstructMesh.dir/ReconstructMesh.cpp.i

apps/ReconstructMesh/CMakeFiles/ReconstructMesh.dir/ReconstructMesh.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ReconstructMesh.dir/ReconstructMesh.cpp.s"
	cd /home/gumbo00/OnlineSFM/scripts/install/openMVS_build/apps/ReconstructMesh && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gumbo00/OnlineSFM/scripts/install/openMVS/apps/ReconstructMesh/ReconstructMesh.cpp -o CMakeFiles/ReconstructMesh.dir/ReconstructMesh.cpp.s

# Object files for target ReconstructMesh
ReconstructMesh_OBJECTS = \
"CMakeFiles/ReconstructMesh.dir/ReconstructMesh.cpp.o"

# External object files for target ReconstructMesh
ReconstructMesh_EXTERNAL_OBJECTS =

bin/ReconstructMesh: apps/ReconstructMesh/CMakeFiles/ReconstructMesh.dir/ReconstructMesh.cpp.o
bin/ReconstructMesh: apps/ReconstructMesh/CMakeFiles/ReconstructMesh.dir/build.make
bin/ReconstructMesh: lib/libMVS.a
bin/ReconstructMesh: lib/libMath.a
bin/ReconstructMesh: lib/libIO.a
bin/ReconstructMesh: lib/libCommon.a
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libboost_iostreams.so.1.71.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libboost_program_options.so.1.71.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libboost_system.so.1.71.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libboost_serialization.so.1.71.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_stitching.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_aruco.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_bgsegm.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_bioinspired.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_ccalib.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_dnn_objdetect.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_dnn_superres.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_dpm.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_highgui.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_face.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_freetype.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_fuzzy.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_hdf.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_hfs.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_img_hash.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_line_descriptor.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_quality.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_reg.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_rgbd.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_saliency.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_shape.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_stereo.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_structured_light.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_phase_unwrapping.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_superres.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_optflow.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_surface_matching.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_tracking.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_datasets.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_plot.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_text.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_dnn.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_ml.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_videostab.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_videoio.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_viz.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_ximgproc.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_video.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_xobjdetect.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_imgcodecs.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_objdetect.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_calib3d.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_features2d.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_flann.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_xphoto.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_photo.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libopencv_core.so.4.2.0
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libpng.so
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libz.so
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libjpeg.so
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libtiff.so
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libmpfr.so
bin/ReconstructMesh: /usr/lib/x86_64-linux-gnu/libgmp.so
bin/ReconstructMesh: apps/ReconstructMesh/CMakeFiles/ReconstructMesh.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/gumbo00/OnlineSFM/scripts/install/openMVS_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/ReconstructMesh"
	cd /home/gumbo00/OnlineSFM/scripts/install/openMVS_build/apps/ReconstructMesh && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ReconstructMesh.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
apps/ReconstructMesh/CMakeFiles/ReconstructMesh.dir/build: bin/ReconstructMesh

.PHONY : apps/ReconstructMesh/CMakeFiles/ReconstructMesh.dir/build

apps/ReconstructMesh/CMakeFiles/ReconstructMesh.dir/clean:
	cd /home/gumbo00/OnlineSFM/scripts/install/openMVS_build/apps/ReconstructMesh && $(CMAKE_COMMAND) -P CMakeFiles/ReconstructMesh.dir/cmake_clean.cmake
.PHONY : apps/ReconstructMesh/CMakeFiles/ReconstructMesh.dir/clean

apps/ReconstructMesh/CMakeFiles/ReconstructMesh.dir/depend:
	cd /home/gumbo00/OnlineSFM/scripts/install/openMVS_build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gumbo00/OnlineSFM/scripts/install/openMVS /home/gumbo00/OnlineSFM/scripts/install/openMVS/apps/ReconstructMesh /home/gumbo00/OnlineSFM/scripts/install/openMVS_build /home/gumbo00/OnlineSFM/scripts/install/openMVS_build/apps/ReconstructMesh /home/gumbo00/OnlineSFM/scripts/install/openMVS_build/apps/ReconstructMesh/CMakeFiles/ReconstructMesh.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/ReconstructMesh/CMakeFiles/ReconstructMesh.dir/depend

