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
include apps/InterfaceCOLMAP/CMakeFiles/InterfaceCOLMAP.dir/depend.make

# Include the progress variables for this target.
include apps/InterfaceCOLMAP/CMakeFiles/InterfaceCOLMAP.dir/progress.make

# Include the compile flags for this target's objects.
include apps/InterfaceCOLMAP/CMakeFiles/InterfaceCOLMAP.dir/flags.make

apps/InterfaceCOLMAP/CMakeFiles/InterfaceCOLMAP.dir/InterfaceCOLMAP.cpp.o: apps/InterfaceCOLMAP/CMakeFiles/InterfaceCOLMAP.dir/flags.make
apps/InterfaceCOLMAP/CMakeFiles/InterfaceCOLMAP.dir/InterfaceCOLMAP.cpp.o: /home/gumbo00/OnlineSFM/scripts/install/openMVS/apps/InterfaceCOLMAP/InterfaceCOLMAP.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gumbo00/OnlineSFM/scripts/install/openMVS_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object apps/InterfaceCOLMAP/CMakeFiles/InterfaceCOLMAP.dir/InterfaceCOLMAP.cpp.o"
	cd /home/gumbo00/OnlineSFM/scripts/install/openMVS_build/apps/InterfaceCOLMAP && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/InterfaceCOLMAP.dir/InterfaceCOLMAP.cpp.o -c /home/gumbo00/OnlineSFM/scripts/install/openMVS/apps/InterfaceCOLMAP/InterfaceCOLMAP.cpp

apps/InterfaceCOLMAP/CMakeFiles/InterfaceCOLMAP.dir/InterfaceCOLMAP.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/InterfaceCOLMAP.dir/InterfaceCOLMAP.cpp.i"
	cd /home/gumbo00/OnlineSFM/scripts/install/openMVS_build/apps/InterfaceCOLMAP && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gumbo00/OnlineSFM/scripts/install/openMVS/apps/InterfaceCOLMAP/InterfaceCOLMAP.cpp > CMakeFiles/InterfaceCOLMAP.dir/InterfaceCOLMAP.cpp.i

apps/InterfaceCOLMAP/CMakeFiles/InterfaceCOLMAP.dir/InterfaceCOLMAP.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/InterfaceCOLMAP.dir/InterfaceCOLMAP.cpp.s"
	cd /home/gumbo00/OnlineSFM/scripts/install/openMVS_build/apps/InterfaceCOLMAP && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gumbo00/OnlineSFM/scripts/install/openMVS/apps/InterfaceCOLMAP/InterfaceCOLMAP.cpp -o CMakeFiles/InterfaceCOLMAP.dir/InterfaceCOLMAP.cpp.s

# Object files for target InterfaceCOLMAP
InterfaceCOLMAP_OBJECTS = \
"CMakeFiles/InterfaceCOLMAP.dir/InterfaceCOLMAP.cpp.o"

# External object files for target InterfaceCOLMAP
InterfaceCOLMAP_EXTERNAL_OBJECTS =

bin/InterfaceCOLMAP: apps/InterfaceCOLMAP/CMakeFiles/InterfaceCOLMAP.dir/InterfaceCOLMAP.cpp.o
bin/InterfaceCOLMAP: apps/InterfaceCOLMAP/CMakeFiles/InterfaceCOLMAP.dir/build.make
bin/InterfaceCOLMAP: lib/libMVS.a
bin/InterfaceCOLMAP: lib/libMath.a
bin/InterfaceCOLMAP: lib/libIO.a
bin/InterfaceCOLMAP: lib/libCommon.a
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libboost_iostreams.so.1.71.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libboost_program_options.so.1.71.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libboost_system.so.1.71.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libboost_serialization.so.1.71.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_stitching.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_aruco.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_bgsegm.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_bioinspired.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_ccalib.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_dnn_objdetect.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_dnn_superres.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_dpm.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_highgui.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_face.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_freetype.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_fuzzy.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_hdf.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_hfs.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_img_hash.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_line_descriptor.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_quality.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_reg.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_rgbd.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_saliency.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_shape.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_stereo.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_structured_light.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_phase_unwrapping.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_superres.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_optflow.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_surface_matching.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_tracking.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_datasets.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_plot.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_text.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_dnn.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_ml.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_videostab.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_videoio.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_viz.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_ximgproc.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_video.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_xobjdetect.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_imgcodecs.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_objdetect.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_calib3d.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_features2d.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_flann.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_xphoto.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_photo.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libopencv_core.so.4.2.0
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libpng.so
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libz.so
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libjpeg.so
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libtiff.so
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libmpfr.so
bin/InterfaceCOLMAP: /usr/lib/x86_64-linux-gnu/libgmp.so
bin/InterfaceCOLMAP: apps/InterfaceCOLMAP/CMakeFiles/InterfaceCOLMAP.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/gumbo00/OnlineSFM/scripts/install/openMVS_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/InterfaceCOLMAP"
	cd /home/gumbo00/OnlineSFM/scripts/install/openMVS_build/apps/InterfaceCOLMAP && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/InterfaceCOLMAP.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
apps/InterfaceCOLMAP/CMakeFiles/InterfaceCOLMAP.dir/build: bin/InterfaceCOLMAP

.PHONY : apps/InterfaceCOLMAP/CMakeFiles/InterfaceCOLMAP.dir/build

apps/InterfaceCOLMAP/CMakeFiles/InterfaceCOLMAP.dir/clean:
	cd /home/gumbo00/OnlineSFM/scripts/install/openMVS_build/apps/InterfaceCOLMAP && $(CMAKE_COMMAND) -P CMakeFiles/InterfaceCOLMAP.dir/cmake_clean.cmake
.PHONY : apps/InterfaceCOLMAP/CMakeFiles/InterfaceCOLMAP.dir/clean

apps/InterfaceCOLMAP/CMakeFiles/InterfaceCOLMAP.dir/depend:
	cd /home/gumbo00/OnlineSFM/scripts/install/openMVS_build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gumbo00/OnlineSFM/scripts/install/openMVS /home/gumbo00/OnlineSFM/scripts/install/openMVS/apps/InterfaceCOLMAP /home/gumbo00/OnlineSFM/scripts/install/openMVS_build /home/gumbo00/OnlineSFM/scripts/install/openMVS_build/apps/InterfaceCOLMAP /home/gumbo00/OnlineSFM/scripts/install/openMVS_build/apps/InterfaceCOLMAP/CMakeFiles/InterfaceCOLMAP.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/InterfaceCOLMAP/CMakeFiles/InterfaceCOLMAP.dir/depend

