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
CMAKE_BINARY_DIR = /home/ukorp/mp_os-main/build

# Include any dependencies generated for this target.
include allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/compiler_depend.make

# Include the progress variables for this target.
include allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/progress.make

# Include the compile flags for this target's objects.
include allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/flags.make

allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_guardant.cpp.o: allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/flags.make
allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_guardant.cpp.o: ../allocator/allocator/src/allocator_guardant.cpp
allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_guardant.cpp.o: allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ukorp/mp_os-main/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_guardant.cpp.o"
	cd /home/ukorp/mp_os-main/build/allocator/allocator && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_guardant.cpp.o -MF CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_guardant.cpp.o.d -o CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_guardant.cpp.o -c /home/ukorp/mp_os-main/allocator/allocator/src/allocator_guardant.cpp

allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_guardant.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_guardant.cpp.i"
	cd /home/ukorp/mp_os-main/build/allocator/allocator && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ukorp/mp_os-main/allocator/allocator/src/allocator_guardant.cpp > CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_guardant.cpp.i

allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_guardant.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_guardant.cpp.s"
	cd /home/ukorp/mp_os-main/build/allocator/allocator && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ukorp/mp_os-main/allocator/allocator/src/allocator_guardant.cpp -o CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_guardant.cpp.s

allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_test_utils.cpp.o: allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/flags.make
allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_test_utils.cpp.o: ../allocator/allocator/src/allocator_test_utils.cpp
allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_test_utils.cpp.o: allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ukorp/mp_os-main/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_test_utils.cpp.o"
	cd /home/ukorp/mp_os-main/build/allocator/allocator && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_test_utils.cpp.o -MF CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_test_utils.cpp.o.d -o CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_test_utils.cpp.o -c /home/ukorp/mp_os-main/allocator/allocator/src/allocator_test_utils.cpp

allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_test_utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_test_utils.cpp.i"
	cd /home/ukorp/mp_os-main/build/allocator/allocator && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ukorp/mp_os-main/allocator/allocator/src/allocator_test_utils.cpp > CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_test_utils.cpp.i

allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_test_utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_test_utils.cpp.s"
	cd /home/ukorp/mp_os-main/build/allocator/allocator && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ukorp/mp_os-main/allocator/allocator/src/allocator_test_utils.cpp -o CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_test_utils.cpp.s

# Object files for target mp_os_allctr_allctr
mp_os_allctr_allctr_OBJECTS = \
"CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_guardant.cpp.o" \
"CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_test_utils.cpp.o"

# External object files for target mp_os_allctr_allctr
mp_os_allctr_allctr_EXTERNAL_OBJECTS =

allocator/allocator/libmp_os_allctr_allctr.a: allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_guardant.cpp.o
allocator/allocator/libmp_os_allctr_allctr.a: allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/src/allocator_test_utils.cpp.o
allocator/allocator/libmp_os_allctr_allctr.a: allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/build.make
allocator/allocator/libmp_os_allctr_allctr.a: allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ukorp/mp_os-main/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libmp_os_allctr_allctr.a"
	cd /home/ukorp/mp_os-main/build/allocator/allocator && $(CMAKE_COMMAND) -P CMakeFiles/mp_os_allctr_allctr.dir/cmake_clean_target.cmake
	cd /home/ukorp/mp_os-main/build/allocator/allocator && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mp_os_allctr_allctr.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/build: allocator/allocator/libmp_os_allctr_allctr.a
.PHONY : allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/build

allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/clean:
	cd /home/ukorp/mp_os-main/build/allocator/allocator && $(CMAKE_COMMAND) -P CMakeFiles/mp_os_allctr_allctr.dir/cmake_clean.cmake
.PHONY : allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/clean

allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/depend:
	cd /home/ukorp/mp_os-main/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ukorp/mp_os-main /home/ukorp/mp_os-main/allocator/allocator /home/ukorp/mp_os-main/build /home/ukorp/mp_os-main/build/allocator/allocator /home/ukorp/mp_os-main/build/allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : allocator/allocator/CMakeFiles/mp_os_allctr_allctr.dir/depend

