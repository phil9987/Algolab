# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

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
CMAKE_SOURCE_DIR = /home/philip/ETHZ/AlgoLab/week3/antenna

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/philip/ETHZ/AlgoLab/week3/antenna

# Include any dependencies generated for this target.
include CMakeFiles/main_fail.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/main_fail.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/main_fail.dir/flags.make

CMakeFiles/main_fail.dir/main_fail.cpp.o: CMakeFiles/main_fail.dir/flags.make
CMakeFiles/main_fail.dir/main_fail.cpp.o: main_fail.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/philip/ETHZ/AlgoLab/week3/antenna/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/main_fail.dir/main_fail.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main_fail.dir/main_fail.cpp.o -c /home/philip/ETHZ/AlgoLab/week3/antenna/main_fail.cpp

CMakeFiles/main_fail.dir/main_fail.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main_fail.dir/main_fail.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/philip/ETHZ/AlgoLab/week3/antenna/main_fail.cpp > CMakeFiles/main_fail.dir/main_fail.cpp.i

CMakeFiles/main_fail.dir/main_fail.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main_fail.dir/main_fail.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/philip/ETHZ/AlgoLab/week3/antenna/main_fail.cpp -o CMakeFiles/main_fail.dir/main_fail.cpp.s

CMakeFiles/main_fail.dir/main_fail.cpp.o.requires:

.PHONY : CMakeFiles/main_fail.dir/main_fail.cpp.o.requires

CMakeFiles/main_fail.dir/main_fail.cpp.o.provides: CMakeFiles/main_fail.dir/main_fail.cpp.o.requires
	$(MAKE) -f CMakeFiles/main_fail.dir/build.make CMakeFiles/main_fail.dir/main_fail.cpp.o.provides.build
.PHONY : CMakeFiles/main_fail.dir/main_fail.cpp.o.provides

CMakeFiles/main_fail.dir/main_fail.cpp.o.provides.build: CMakeFiles/main_fail.dir/main_fail.cpp.o


# Object files for target main_fail
main_fail_OBJECTS = \
"CMakeFiles/main_fail.dir/main_fail.cpp.o"

# External object files for target main_fail
main_fail_EXTERNAL_OBJECTS =

main_fail: CMakeFiles/main_fail.dir/main_fail.cpp.o
main_fail: CMakeFiles/main_fail.dir/build.make
main_fail: /usr/lib/x86_64-linux-gnu/libmpfr.so
main_fail: /usr/lib/x86_64-linux-gnu/libgmp.so
main_fail: /usr/lib/x86_64-linux-gnu/libCGAL_Core.so.12.0.0
main_fail: /usr/lib/x86_64-linux-gnu/libCGAL.so.12.0.0
main_fail: /usr/lib/x86_64-linux-gnu/libCGAL_Core.so.12.0.0
main_fail: /usr/lib/x86_64-linux-gnu/libCGAL.so.12.0.0
main_fail: CMakeFiles/main_fail.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/philip/ETHZ/AlgoLab/week3/antenna/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable main_fail"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main_fail.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/main_fail.dir/build: main_fail

.PHONY : CMakeFiles/main_fail.dir/build

CMakeFiles/main_fail.dir/requires: CMakeFiles/main_fail.dir/main_fail.cpp.o.requires

.PHONY : CMakeFiles/main_fail.dir/requires

CMakeFiles/main_fail.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/main_fail.dir/cmake_clean.cmake
.PHONY : CMakeFiles/main_fail.dir/clean

CMakeFiles/main_fail.dir/depend:
	cd /home/philip/ETHZ/AlgoLab/week3/antenna && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/philip/ETHZ/AlgoLab/week3/antenna /home/philip/ETHZ/AlgoLab/week3/antenna /home/philip/ETHZ/AlgoLab/week3/antenna /home/philip/ETHZ/AlgoLab/week3/antenna /home/philip/ETHZ/AlgoLab/week3/antenna/CMakeFiles/main_fail.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/main_fail.dir/depend

