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
CMAKE_SOURCE_DIR = /home/ukorp/mp_os-main

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ukorp/mp_os-main/cmake-build-release

# Include any dependencies generated for this target.
include allocator/allocator_sorted_list/CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include allocator/allocator_sorted_list/CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/compiler_depend.make

# Include the progress variables for this target.
include allocator/allocator_sorted_list/CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/progress.make

# Include the compile flags for this target's objects.
include allocator/allocator_sorted_list/CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/flags.make

allocator/allocator_sorted_list/CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/src/allocator_sorted_list.cpp.o: allocator/allocator_sorted_list/CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/flags.make
allocator/allocator_sorted_list/CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/src/allocator_sorted_list.cpp.o: ../allocator/allocator_sorted_list/src/allocator_sorted_list.cpp
allocator/allocator_sorted_list/CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/src/allocator_sorted_list.cpp.o: allocator/allocator_sorted_list/CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ukorp/mp_os-main/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object allocator/allocator_sorted_list/CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/src/allocator_sorted_list.cpp.o"
	cd /home/ukorp/mp_os-main/cmake-build-release/allocator/allocator_sorted_list && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT allocator/allocator_sorted_list/CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/src/allocator_sorted_list.cpp.o -MF CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/src/allocator_sorted_list.cpp.o.d -o CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/src/allocator_sorted_list.cpp.o -c /home/ukorp/mp_os-main/allocator/allocator_sorted_list/src/allocator_sorted_list.cpp

allocator/allocator_sorted_list/CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/src/allocator_sorted_list.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/src/allocator_sorted_list.cpp.i"
	cd /home/ukorp/mp_os-main/cmake-build-release/allocator/allocator_sorted_list && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ukorp/mp_os-main/allocator/allocator_sorted_list/src/allocator_sorted_list.cpp > CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/src/allocator_sorted_list.cpp.i

allocator/allocator_sorted_list/CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/src/allocator_sorted_list.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/src/allocator_sorted_list.cpp.s"
	cd /home/ukorp/mp_os-main/cmake-build-release/allocator/allocator_sorted_list && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ukorp/mp_os-main/allocator/allocator_sorted_list/src/allocator_sorted_list.cpp -o CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/src/allocator_sorted_list.cpp.s

# Object files for target mp_os_allctr_allctr_srtd_lst
mp_os_allctr_allctr_srtd_lst_OBJECTS = \
"CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/src/allocator_sorted_list.cpp.o"

# External object files for target mp_os_allctr_allctr_srtd_lst
mp_os_allctr_allctr_srtd_lst_EXTERNAL_OBJECTS =

allocator/allocator_sorted_list/libmp_os_allctr_allctr_srtd_lst.a: allocator/allocator_sorted_list/CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/src/allocator_sorted_list.cpp.o
allocator/allocator_sorted_list/libmp_os_allctr_allctr_srtd_lst.a: allocator/allocator_sorted_list/CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/build.make
allocator/allocator_sorted_list/libmp_os_allctr_allctr_srtd_lst.a: allocator/allocator_sorted_list/CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ukorp/mp_os-main/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libmp_os_allctr_allctr_srtd_lst.a"
	cd /home/ukorp/mp_os-main/cmake-build-release/allocator/allocator_sorted_list && $(CMAKE_COMMAND) -P CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/cmake_clean_target.cmake
	cd /home/ukorp/mp_os-main/cmake-build-release/allocator/allocator_sorted_list && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
allocator/allocator_sorted_list/CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/build: allocator/allocator_sorted_list/libmp_os_allctr_allctr_srtd_lst.a
.PHONY : allocator/allocator_sorted_list/CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/build

allocator/allocator_sorted_list/CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/clean:
	cd /home/ukorp/mp_os-main/cmake-build-release/allocator/allocator_sorted_list && $(CMAKE_COMMAND) -P CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/cmake_clean.cmake
.PHONY : allocator/allocator_sorted_list/CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/clean

allocator/allocator_sorted_list/CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/depend:
	cd /home/ukorp/mp_os-main/cmake-build-release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ukorp/mp_os-main /home/ukorp/mp_os-main/allocator/allocator_sorted_list /home/ukorp/mp_os-main/cmake-build-release /home/ukorp/mp_os-main/cmake-build-release/allocator/allocator_sorted_list /home/ukorp/mp_os-main/cmake-build-release/allocator/allocator_sorted_list/CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : allocator/allocator_sorted_list/CMakeFiles/mp_os_allctr_allctr_srtd_lst.dir/depend

