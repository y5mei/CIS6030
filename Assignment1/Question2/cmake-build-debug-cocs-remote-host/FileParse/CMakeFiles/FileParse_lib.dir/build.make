# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_SOURCE_DIR = /home/undergrad/2/ymei/workspace/A1-2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/undergrad/2/ymei/workspace/A1-2/cmake-build-debug-cocs-remote-host

# Include any dependencies generated for this target.
include FileParse/CMakeFiles/FileParse_lib.dir/depend.make

# Include the progress variables for this target.
include FileParse/CMakeFiles/FileParse_lib.dir/progress.make

# Include the compile flags for this target's objects.
include FileParse/CMakeFiles/FileParse_lib.dir/flags.make

FileParse/CMakeFiles/FileParse_lib.dir/ReadRawFile.cpp.o: FileParse/CMakeFiles/FileParse_lib.dir/flags.make
FileParse/CMakeFiles/FileParse_lib.dir/ReadRawFile.cpp.o: ../FileParse/ReadRawFile.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/undergrad/2/ymei/workspace/A1-2/cmake-build-debug-cocs-remote-host/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object FileParse/CMakeFiles/FileParse_lib.dir/ReadRawFile.cpp.o"
	cd /home/undergrad/2/ymei/workspace/A1-2/cmake-build-debug-cocs-remote-host/FileParse && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FileParse_lib.dir/ReadRawFile.cpp.o -c /home/undergrad/2/ymei/workspace/A1-2/FileParse/ReadRawFile.cpp

FileParse/CMakeFiles/FileParse_lib.dir/ReadRawFile.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FileParse_lib.dir/ReadRawFile.cpp.i"
	cd /home/undergrad/2/ymei/workspace/A1-2/cmake-build-debug-cocs-remote-host/FileParse && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/undergrad/2/ymei/workspace/A1-2/FileParse/ReadRawFile.cpp > CMakeFiles/FileParse_lib.dir/ReadRawFile.cpp.i

FileParse/CMakeFiles/FileParse_lib.dir/ReadRawFile.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FileParse_lib.dir/ReadRawFile.cpp.s"
	cd /home/undergrad/2/ymei/workspace/A1-2/cmake-build-debug-cocs-remote-host/FileParse && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/undergrad/2/ymei/workspace/A1-2/FileParse/ReadRawFile.cpp -o CMakeFiles/FileParse_lib.dir/ReadRawFile.cpp.s

# Object files for target FileParse_lib
FileParse_lib_OBJECTS = \
"CMakeFiles/FileParse_lib.dir/ReadRawFile.cpp.o"

# External object files for target FileParse_lib
FileParse_lib_EXTERNAL_OBJECTS =

FileParse/libFileParse_lib.a: FileParse/CMakeFiles/FileParse_lib.dir/ReadRawFile.cpp.o
FileParse/libFileParse_lib.a: FileParse/CMakeFiles/FileParse_lib.dir/build.make
FileParse/libFileParse_lib.a: FileParse/CMakeFiles/FileParse_lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/undergrad/2/ymei/workspace/A1-2/cmake-build-debug-cocs-remote-host/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libFileParse_lib.a"
	cd /home/undergrad/2/ymei/workspace/A1-2/cmake-build-debug-cocs-remote-host/FileParse && $(CMAKE_COMMAND) -P CMakeFiles/FileParse_lib.dir/cmake_clean_target.cmake
	cd /home/undergrad/2/ymei/workspace/A1-2/cmake-build-debug-cocs-remote-host/FileParse && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FileParse_lib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
FileParse/CMakeFiles/FileParse_lib.dir/build: FileParse/libFileParse_lib.a

.PHONY : FileParse/CMakeFiles/FileParse_lib.dir/build

FileParse/CMakeFiles/FileParse_lib.dir/clean:
	cd /home/undergrad/2/ymei/workspace/A1-2/cmake-build-debug-cocs-remote-host/FileParse && $(CMAKE_COMMAND) -P CMakeFiles/FileParse_lib.dir/cmake_clean.cmake
.PHONY : FileParse/CMakeFiles/FileParse_lib.dir/clean

FileParse/CMakeFiles/FileParse_lib.dir/depend:
	cd /home/undergrad/2/ymei/workspace/A1-2/cmake-build-debug-cocs-remote-host && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/undergrad/2/ymei/workspace/A1-2 /home/undergrad/2/ymei/workspace/A1-2/FileParse /home/undergrad/2/ymei/workspace/A1-2/cmake-build-debug-cocs-remote-host /home/undergrad/2/ymei/workspace/A1-2/cmake-build-debug-cocs-remote-host/FileParse /home/undergrad/2/ymei/workspace/A1-2/cmake-build-debug-cocs-remote-host/FileParse/CMakeFiles/FileParse_lib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : FileParse/CMakeFiles/FileParse_lib.dir/depend

