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
CMAKE_SOURCE_DIR = /home/philip/ETHZ/AlgoLab/week3/cgal_tryout2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/philip/ETHZ/AlgoLab/week3/cgal_tryout2

# Include any dependencies generated for this target.
include CMakeFiles/sample2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/sample2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sample2.dir/flags.make

CMakeFiles/sample2.dir/sample2.cpp.o: CMakeFiles/sample2.dir/flags.make
CMakeFiles/sample2.dir/sample2.cpp.o: sample2.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/philip/ETHZ/AlgoLab/week3/cgal_tryout2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/sample2.dir/sample2.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sample2.dir/sample2.cpp.o -c /home/philip/ETHZ/AlgoLab/week3/cgal_tryout2/sample2.cpp

CMakeFiles/sample2.dir/sample2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sample2.dir/sample2.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/philip/ETHZ/AlgoLab/week3/cgal_tryout2/sample2.cpp > CMakeFiles/sample2.dir/sample2.cpp.i

CMakeFiles/sample2.dir/sample2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sample2.dir/sample2.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/philip/ETHZ/AlgoLab/week3/cgal_tryout2/sample2.cpp -o CMakeFiles/sample2.dir/sample2.cpp.s

CMakeFiles/sample2.dir/sample2.cpp.o.requires:

.PHONY : CMakeFiles/sample2.dir/sample2.cpp.o.requires

CMakeFiles/sample2.dir/sample2.cpp.o.provides: CMakeFiles/sample2.dir/sample2.cpp.o.requires
	$(MAKE) -f CMakeFiles/sample2.dir/build.make CMakeFiles/sample2.dir/sample2.cpp.o.provides.build
.PHONY : CMakeFiles/sample2.dir/sample2.cpp.o.provides

CMakeFiles/sample2.dir/sample2.cpp.o.provides.build: CMakeFiles/sample2.dir/sample2.cpp.o


# Object files for target sample2
sample2_OBJECTS = \
"CMakeFiles/sample2.dir/sample2.cpp.o"

# External object files for target sample2
sample2_EXTERNAL_OBJECTS =

sample2: CMakeFiles/sample2.dir/sample2.cpp.o
sample2: CMakeFiles/sample2.dir/build.make
sample2: /usr/lib/x86_64-linux-gnu/libmpfr.so
sample2: /usr/lib/x86_64-linux-gnu/libgmp.so
sample2: /usr/lib/x86_64-linux-gnu/libCGAL_Core.so.12.0.0
sample2: /usr/lib/x86_64-linux-gnu/libCGAL.so.12.0.0
sample2: /usr/lib/x86_64-linux-gnu/libCGAL_Core.so.12.0.0
sample2: /usr/lib/x86_64-linux-gnu/libCGAL.so.12.0.0
sample2: CMakeFiles/sample2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/philip/ETHZ/AlgoLab/week3/cgal_tryout2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable sample2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sample2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sample2.dir/build: sample2

.PHONY : CMakeFiles/sample2.dir/build

CMakeFiles/sample2.dir/requires: CMakeFiles/sample2.dir/sample2.cpp.o.requires

.PHONY : CMakeFiles/sample2.dir/requires

CMakeFiles/sample2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sample2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sample2.dir/clean

CMakeFiles/sample2.dir/depend:
	cd /home/philip/ETHZ/AlgoLab/week3/cgal_tryout2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/philip/ETHZ/AlgoLab/week3/cgal_tryout2 /home/philip/ETHZ/AlgoLab/week3/cgal_tryout2 /home/philip/ETHZ/AlgoLab/week3/cgal_tryout2 /home/philip/ETHZ/AlgoLab/week3/cgal_tryout2 /home/philip/ETHZ/AlgoLab/week3/cgal_tryout2/CMakeFiles/sample2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sample2.dir/depend
