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
CMAKE_SOURCE_DIR = /home/philip/ETHZ/AlgoLab/week4/pow

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/philip/ETHZ/AlgoLab/week4/pow

# Include any dependencies generated for this target.
include CMakeFiles/main2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/main2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/main2.dir/flags.make

CMakeFiles/main2.dir/main2.cpp.o: CMakeFiles/main2.dir/flags.make
CMakeFiles/main2.dir/main2.cpp.o: main2.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/philip/ETHZ/AlgoLab/week4/pow/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/main2.dir/main2.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main2.dir/main2.cpp.o -c /home/philip/ETHZ/AlgoLab/week4/pow/main2.cpp

CMakeFiles/main2.dir/main2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main2.dir/main2.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/philip/ETHZ/AlgoLab/week4/pow/main2.cpp > CMakeFiles/main2.dir/main2.cpp.i

CMakeFiles/main2.dir/main2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main2.dir/main2.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/philip/ETHZ/AlgoLab/week4/pow/main2.cpp -o CMakeFiles/main2.dir/main2.cpp.s

CMakeFiles/main2.dir/main2.cpp.o.requires:

.PHONY : CMakeFiles/main2.dir/main2.cpp.o.requires

CMakeFiles/main2.dir/main2.cpp.o.provides: CMakeFiles/main2.dir/main2.cpp.o.requires
	$(MAKE) -f CMakeFiles/main2.dir/build.make CMakeFiles/main2.dir/main2.cpp.o.provides.build
.PHONY : CMakeFiles/main2.dir/main2.cpp.o.provides

CMakeFiles/main2.dir/main2.cpp.o.provides.build: CMakeFiles/main2.dir/main2.cpp.o


# Object files for target main2
main2_OBJECTS = \
"CMakeFiles/main2.dir/main2.cpp.o"

# External object files for target main2
main2_EXTERNAL_OBJECTS =

main2: CMakeFiles/main2.dir/main2.cpp.o
main2: CMakeFiles/main2.dir/build.make
main2: /usr/lib/x86_64-linux-gnu/libmpfr.so
main2: /usr/lib/x86_64-linux-gnu/libgmp.so
main2: /usr/lib/x86_64-linux-gnu/libCGAL_Core.so.12.0.0
main2: /usr/lib/x86_64-linux-gnu/libCGAL.so.12.0.0
main2: /usr/lib/x86_64-linux-gnu/libCGAL_Core.so.12.0.0
main2: /usr/lib/x86_64-linux-gnu/libCGAL.so.12.0.0
main2: CMakeFiles/main2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/philip/ETHZ/AlgoLab/week4/pow/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable main2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/main2.dir/build: main2

.PHONY : CMakeFiles/main2.dir/build

CMakeFiles/main2.dir/requires: CMakeFiles/main2.dir/main2.cpp.o.requires

.PHONY : CMakeFiles/main2.dir/requires

CMakeFiles/main2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/main2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/main2.dir/clean

CMakeFiles/main2.dir/depend:
	cd /home/philip/ETHZ/AlgoLab/week4/pow && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/philip/ETHZ/AlgoLab/week4/pow /home/philip/ETHZ/AlgoLab/week4/pow /home/philip/ETHZ/AlgoLab/week4/pow /home/philip/ETHZ/AlgoLab/week4/pow /home/philip/ETHZ/AlgoLab/week4/pow/CMakeFiles/main2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/main2.dir/depend

