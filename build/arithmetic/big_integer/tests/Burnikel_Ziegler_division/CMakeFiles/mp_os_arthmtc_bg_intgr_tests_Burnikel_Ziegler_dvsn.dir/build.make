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
include arithmetic/big_integer/tests/Burnikel_Ziegler_division/CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include arithmetic/big_integer/tests/Burnikel_Ziegler_division/CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/compiler_depend.make

# Include the progress variables for this target.
include arithmetic/big_integer/tests/Burnikel_Ziegler_division/CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/progress.make

# Include the compile flags for this target's objects.
include arithmetic/big_integer/tests/Burnikel_Ziegler_division/CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/flags.make

arithmetic/big_integer/tests/Burnikel_Ziegler_division/CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/Burnikel_Ziegler_division_tests.cpp.o: arithmetic/big_integer/tests/Burnikel_Ziegler_division/CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/flags.make
arithmetic/big_integer/tests/Burnikel_Ziegler_division/CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/Burnikel_Ziegler_division_tests.cpp.o: ../arithmetic/big_integer/tests/Burnikel_Ziegler_division/Burnikel_Ziegler_division_tests.cpp
arithmetic/big_integer/tests/Burnikel_Ziegler_division/CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/Burnikel_Ziegler_division_tests.cpp.o: arithmetic/big_integer/tests/Burnikel_Ziegler_division/CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ukorp/mp_os-main/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object arithmetic/big_integer/tests/Burnikel_Ziegler_division/CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/Burnikel_Ziegler_division_tests.cpp.o"
	cd /home/ukorp/mp_os-main/build/arithmetic/big_integer/tests/Burnikel_Ziegler_division && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT arithmetic/big_integer/tests/Burnikel_Ziegler_division/CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/Burnikel_Ziegler_division_tests.cpp.o -MF CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/Burnikel_Ziegler_division_tests.cpp.o.d -o CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/Burnikel_Ziegler_division_tests.cpp.o -c /home/ukorp/mp_os-main/arithmetic/big_integer/tests/Burnikel_Ziegler_division/Burnikel_Ziegler_division_tests.cpp

arithmetic/big_integer/tests/Burnikel_Ziegler_division/CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/Burnikel_Ziegler_division_tests.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/Burnikel_Ziegler_division_tests.cpp.i"
	cd /home/ukorp/mp_os-main/build/arithmetic/big_integer/tests/Burnikel_Ziegler_division && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ukorp/mp_os-main/arithmetic/big_integer/tests/Burnikel_Ziegler_division/Burnikel_Ziegler_division_tests.cpp > CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/Burnikel_Ziegler_division_tests.cpp.i

arithmetic/big_integer/tests/Burnikel_Ziegler_division/CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/Burnikel_Ziegler_division_tests.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/Burnikel_Ziegler_division_tests.cpp.s"
	cd /home/ukorp/mp_os-main/build/arithmetic/big_integer/tests/Burnikel_Ziegler_division && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ukorp/mp_os-main/arithmetic/big_integer/tests/Burnikel_Ziegler_division/Burnikel_Ziegler_division_tests.cpp -o CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/Burnikel_Ziegler_division_tests.cpp.s

# Object files for target mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn
mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn_OBJECTS = \
"CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/Burnikel_Ziegler_division_tests.cpp.o"

# External object files for target mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn
mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn_EXTERNAL_OBJECTS =

arithmetic/big_integer/tests/Burnikel_Ziegler_division/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn-1.0: arithmetic/big_integer/tests/Burnikel_Ziegler_division/CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/Burnikel_Ziegler_division_tests.cpp.o
arithmetic/big_integer/tests/Burnikel_Ziegler_division/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn-1.0: arithmetic/big_integer/tests/Burnikel_Ziegler_division/CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/build.make
arithmetic/big_integer/tests/Burnikel_Ziegler_division/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn-1.0: lib/libgtest_main.a
arithmetic/big_integer/tests/Burnikel_Ziegler_division/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn-1.0: common/libmp_os_cmmn.a
arithmetic/big_integer/tests/Burnikel_Ziegler_division/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn-1.0: logger/client_logger/libmp_os_lggr_clnt_lggr.a
arithmetic/big_integer/tests/Burnikel_Ziegler_division/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn-1.0: arithmetic/big_integer/libmp_os_arthmtc_bg_intgr.a
arithmetic/big_integer/tests/Burnikel_Ziegler_division/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn-1.0: lib/libgtest.a
arithmetic/big_integer/tests/Burnikel_Ziegler_division/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn-1.0: logger/logger/libmp_os_lggr_lggr.a
arithmetic/big_integer/tests/Burnikel_Ziegler_division/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn-1.0: common/libmp_os_cmmn.a
arithmetic/big_integer/tests/Burnikel_Ziegler_division/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn-1.0: allocator/allocator/libmp_os_allctr_allctr.a
arithmetic/big_integer/tests/Burnikel_Ziegler_division/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn-1.0: arithmetic/big_integer/tests/Burnikel_Ziegler_division/CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ukorp/mp_os-main/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn"
	cd /home/ukorp/mp_os-main/build/arithmetic/big_integer/tests/Burnikel_Ziegler_division && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/link.txt --verbose=$(VERBOSE)
	cd /home/ukorp/mp_os-main/build/arithmetic/big_integer/tests/Burnikel_Ziegler_division && $(CMAKE_COMMAND) -E cmake_symlink_executable mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn-1.0 mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn

arithmetic/big_integer/tests/Burnikel_Ziegler_division/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn: arithmetic/big_integer/tests/Burnikel_Ziegler_division/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn-1.0

# Rule to build all files generated by this target.
arithmetic/big_integer/tests/Burnikel_Ziegler_division/CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/build: arithmetic/big_integer/tests/Burnikel_Ziegler_division/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn
.PHONY : arithmetic/big_integer/tests/Burnikel_Ziegler_division/CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/build

arithmetic/big_integer/tests/Burnikel_Ziegler_division/CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/clean:
	cd /home/ukorp/mp_os-main/build/arithmetic/big_integer/tests/Burnikel_Ziegler_division && $(CMAKE_COMMAND) -P CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/cmake_clean.cmake
.PHONY : arithmetic/big_integer/tests/Burnikel_Ziegler_division/CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/clean

arithmetic/big_integer/tests/Burnikel_Ziegler_division/CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/depend:
	cd /home/ukorp/mp_os-main/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ukorp/mp_os-main /home/ukorp/mp_os-main/arithmetic/big_integer/tests/Burnikel_Ziegler_division /home/ukorp/mp_os-main/build /home/ukorp/mp_os-main/build/arithmetic/big_integer/tests/Burnikel_Ziegler_division /home/ukorp/mp_os-main/build/arithmetic/big_integer/tests/Burnikel_Ziegler_division/CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : arithmetic/big_integer/tests/Burnikel_Ziegler_division/CMakeFiles/mp_os_arthmtc_bg_intgr_tests_Burnikel_Ziegler_dvsn.dir/depend

