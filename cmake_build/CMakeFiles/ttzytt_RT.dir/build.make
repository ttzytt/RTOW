# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/e/prog/c++/RTOW

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/e/prog/c++/RTOW/cmake_build

# Include any dependencies generated for this target.
include CMakeFiles/ttzytt_RT.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/ttzytt_RT.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ttzytt_RT.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ttzytt_RT.dir/flags.make

CMakeFiles/ttzytt_RT.dir/main.cpp.o: CMakeFiles/ttzytt_RT.dir/flags.make
CMakeFiles/ttzytt_RT.dir/main.cpp.o: ../main.cpp
CMakeFiles/ttzytt_RT.dir/main.cpp.o: CMakeFiles/ttzytt_RT.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/e/prog/c++/RTOW/cmake_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ttzytt_RT.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ttzytt_RT.dir/main.cpp.o -MF CMakeFiles/ttzytt_RT.dir/main.cpp.o.d -o CMakeFiles/ttzytt_RT.dir/main.cpp.o -c /mnt/e/prog/c++/RTOW/main.cpp

CMakeFiles/ttzytt_RT.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ttzytt_RT.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/e/prog/c++/RTOW/main.cpp > CMakeFiles/ttzytt_RT.dir/main.cpp.i

CMakeFiles/ttzytt_RT.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ttzytt_RT.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/e/prog/c++/RTOW/main.cpp -o CMakeFiles/ttzytt_RT.dir/main.cpp.s

# Object files for target ttzytt_RT
ttzytt_RT_OBJECTS = \
"CMakeFiles/ttzytt_RT.dir/main.cpp.o"

# External object files for target ttzytt_RT
ttzytt_RT_EXTERNAL_OBJECTS =

ttzytt_RT: CMakeFiles/ttzytt_RT.dir/main.cpp.o
ttzytt_RT: CMakeFiles/ttzytt_RT.dir/build.make
ttzytt_RT: CMakeFiles/ttzytt_RT.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/e/prog/c++/RTOW/cmake_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ttzytt_RT"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ttzytt_RT.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ttzytt_RT.dir/build: ttzytt_RT
.PHONY : CMakeFiles/ttzytt_RT.dir/build

CMakeFiles/ttzytt_RT.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ttzytt_RT.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ttzytt_RT.dir/clean

CMakeFiles/ttzytt_RT.dir/depend:
	cd /mnt/e/prog/c++/RTOW/cmake_build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/e/prog/c++/RTOW /mnt/e/prog/c++/RTOW /mnt/e/prog/c++/RTOW/cmake_build /mnt/e/prog/c++/RTOW/cmake_build /mnt/e/prog/c++/RTOW/cmake_build/CMakeFiles/ttzytt_RT.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ttzytt_RT.dir/depend

