# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/serverDemo/src/server_single

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/serverDemo/src/server_single

# Include any dependencies generated for this target.
include CMakeFiles/server_single.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/server_single.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/server_single.dir/flags.make

CMakeFiles/server_single.dir/main.cpp.o: CMakeFiles/server_single.dir/flags.make
CMakeFiles/server_single.dir/main.cpp.o: main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /root/serverDemo/src/server_single/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/server_single.dir/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/server_single.dir/main.cpp.o -c /root/serverDemo/src/server_single/main.cpp

CMakeFiles/server_single.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server_single.dir/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /root/serverDemo/src/server_single/main.cpp > CMakeFiles/server_single.dir/main.cpp.i

CMakeFiles/server_single.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server_single.dir/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /root/serverDemo/src/server_single/main.cpp -o CMakeFiles/server_single.dir/main.cpp.s

CMakeFiles/server_single.dir/main.cpp.o.requires:
.PHONY : CMakeFiles/server_single.dir/main.cpp.o.requires

CMakeFiles/server_single.dir/main.cpp.o.provides: CMakeFiles/server_single.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/server_single.dir/build.make CMakeFiles/server_single.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/server_single.dir/main.cpp.o.provides

CMakeFiles/server_single.dir/main.cpp.o.provides.build: CMakeFiles/server_single.dir/main.cpp.o

# Object files for target server_single
server_single_OBJECTS = \
"CMakeFiles/server_single.dir/main.cpp.o"

# External object files for target server_single
server_single_EXTERNAL_OBJECTS =

server_single: CMakeFiles/server_single.dir/main.cpp.o
server_single: CMakeFiles/server_single.dir/build.make
server_single: CMakeFiles/server_single.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable server_single"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server_single.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/server_single.dir/build: server_single
.PHONY : CMakeFiles/server_single.dir/build

CMakeFiles/server_single.dir/requires: CMakeFiles/server_single.dir/main.cpp.o.requires
.PHONY : CMakeFiles/server_single.dir/requires

CMakeFiles/server_single.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/server_single.dir/cmake_clean.cmake
.PHONY : CMakeFiles/server_single.dir/clean

CMakeFiles/server_single.dir/depend:
	cd /root/serverDemo/src/server_single && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/serverDemo/src/server_single /root/serverDemo/src/server_single /root/serverDemo/src/server_single /root/serverDemo/src/server_single /root/serverDemo/src/server_single/CMakeFiles/server_single.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/server_single.dir/depend

