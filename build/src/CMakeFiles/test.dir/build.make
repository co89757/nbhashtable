# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.4

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/colin/Documents/Courses/multicore-522/Project/NBHashTable

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/build

# Include any dependencies generated for this target.
include src/CMakeFiles/test.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/test.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/test.dir/flags.make

src/CMakeFiles/test.dir/testHashMap.o: src/CMakeFiles/test.dir/flags.make
src/CMakeFiles/test.dir/testHashMap.o: ../src/testHashMap.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/colin/Documents/Courses/multicore-522/Project/NBHashTable/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/CMakeFiles/test.dir/testHashMap.o"
	cd /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/build/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/test.dir/testHashMap.o   -c /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/src/testHashMap.c

src/CMakeFiles/test.dir/testHashMap.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test.dir/testHashMap.i"
	cd /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/build/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/src/testHashMap.c > CMakeFiles/test.dir/testHashMap.i

src/CMakeFiles/test.dir/testHashMap.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test.dir/testHashMap.s"
	cd /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/build/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/src/testHashMap.c -o CMakeFiles/test.dir/testHashMap.s

src/CMakeFiles/test.dir/testHashMap.o.requires:

.PHONY : src/CMakeFiles/test.dir/testHashMap.o.requires

src/CMakeFiles/test.dir/testHashMap.o.provides: src/CMakeFiles/test.dir/testHashMap.o.requires
	$(MAKE) -f src/CMakeFiles/test.dir/build.make src/CMakeFiles/test.dir/testHashMap.o.provides.build
.PHONY : src/CMakeFiles/test.dir/testHashMap.o.provides

src/CMakeFiles/test.dir/testHashMap.o.provides.build: src/CMakeFiles/test.dir/testHashMap.o


src/CMakeFiles/test.dir/nbhash.o: src/CMakeFiles/test.dir/flags.make
src/CMakeFiles/test.dir/nbhash.o: ../src/nbhash.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/colin/Documents/Courses/multicore-522/Project/NBHashTable/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object src/CMakeFiles/test.dir/nbhash.o"
	cd /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/build/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/test.dir/nbhash.o   -c /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/src/nbhash.c

src/CMakeFiles/test.dir/nbhash.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test.dir/nbhash.i"
	cd /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/build/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/src/nbhash.c > CMakeFiles/test.dir/nbhash.i

src/CMakeFiles/test.dir/nbhash.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test.dir/nbhash.s"
	cd /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/build/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/src/nbhash.c -o CMakeFiles/test.dir/nbhash.s

src/CMakeFiles/test.dir/nbhash.o.requires:

.PHONY : src/CMakeFiles/test.dir/nbhash.o.requires

src/CMakeFiles/test.dir/nbhash.o.provides: src/CMakeFiles/test.dir/nbhash.o.requires
	$(MAKE) -f src/CMakeFiles/test.dir/build.make src/CMakeFiles/test.dir/nbhash.o.provides.build
.PHONY : src/CMakeFiles/test.dir/nbhash.o.provides

src/CMakeFiles/test.dir/nbhash.o.provides.build: src/CMakeFiles/test.dir/nbhash.o


src/CMakeFiles/test.dir/collection.o: src/CMakeFiles/test.dir/flags.make
src/CMakeFiles/test.dir/collection.o: ../src/collection.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/colin/Documents/Courses/multicore-522/Project/NBHashTable/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object src/CMakeFiles/test.dir/collection.o"
	cd /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/build/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/test.dir/collection.o   -c /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/src/collection.c

src/CMakeFiles/test.dir/collection.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test.dir/collection.i"
	cd /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/build/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/src/collection.c > CMakeFiles/test.dir/collection.i

src/CMakeFiles/test.dir/collection.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test.dir/collection.s"
	cd /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/build/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/src/collection.c -o CMakeFiles/test.dir/collection.s

src/CMakeFiles/test.dir/collection.o.requires:

.PHONY : src/CMakeFiles/test.dir/collection.o.requires

src/CMakeFiles/test.dir/collection.o.provides: src/CMakeFiles/test.dir/collection.o.requires
	$(MAKE) -f src/CMakeFiles/test.dir/build.make src/CMakeFiles/test.dir/collection.o.provides.build
.PHONY : src/CMakeFiles/test.dir/collection.o.provides

src/CMakeFiles/test.dir/collection.o.provides.build: src/CMakeFiles/test.dir/collection.o


src/CMakeFiles/test.dir/nstring.o: src/CMakeFiles/test.dir/flags.make
src/CMakeFiles/test.dir/nstring.o: ../src/nstring.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/colin/Documents/Courses/multicore-522/Project/NBHashTable/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object src/CMakeFiles/test.dir/nstring.o"
	cd /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/build/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/test.dir/nstring.o   -c /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/src/nstring.c

src/CMakeFiles/test.dir/nstring.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test.dir/nstring.i"
	cd /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/build/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/src/nstring.c > CMakeFiles/test.dir/nstring.i

src/CMakeFiles/test.dir/nstring.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test.dir/nstring.s"
	cd /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/build/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/src/nstring.c -o CMakeFiles/test.dir/nstring.s

src/CMakeFiles/test.dir/nstring.o.requires:

.PHONY : src/CMakeFiles/test.dir/nstring.o.requires

src/CMakeFiles/test.dir/nstring.o.provides: src/CMakeFiles/test.dir/nstring.o.requires
	$(MAKE) -f src/CMakeFiles/test.dir/build.make src/CMakeFiles/test.dir/nstring.o.provides.build
.PHONY : src/CMakeFiles/test.dir/nstring.o.provides

src/CMakeFiles/test.dir/nstring.o.provides.build: src/CMakeFiles/test.dir/nstring.o


# Object files for target test
test_OBJECTS = \
"CMakeFiles/test.dir/testHashMap.o" \
"CMakeFiles/test.dir/nbhash.o" \
"CMakeFiles/test.dir/collection.o" \
"CMakeFiles/test.dir/nstring.o"

# External object files for target test
test_EXTERNAL_OBJECTS =

bin/test: src/CMakeFiles/test.dir/testHashMap.o
bin/test: src/CMakeFiles/test.dir/nbhash.o
bin/test: src/CMakeFiles/test.dir/collection.o
bin/test: src/CMakeFiles/test.dir/nstring.o
bin/test: src/CMakeFiles/test.dir/build.make
bin/test: src/CMakeFiles/test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/colin/Documents/Courses/multicore-522/Project/NBHashTable/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C executable ../bin/test"
	cd /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/test.dir/build: bin/test

.PHONY : src/CMakeFiles/test.dir/build

src/CMakeFiles/test.dir/requires: src/CMakeFiles/test.dir/testHashMap.o.requires
src/CMakeFiles/test.dir/requires: src/CMakeFiles/test.dir/nbhash.o.requires
src/CMakeFiles/test.dir/requires: src/CMakeFiles/test.dir/collection.o.requires
src/CMakeFiles/test.dir/requires: src/CMakeFiles/test.dir/nstring.o.requires

.PHONY : src/CMakeFiles/test.dir/requires

src/CMakeFiles/test.dir/clean:
	cd /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/build/src && $(CMAKE_COMMAND) -P CMakeFiles/test.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/test.dir/clean

src/CMakeFiles/test.dir/depend:
	cd /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/colin/Documents/Courses/multicore-522/Project/NBHashTable /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/src /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/build /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/build/src /home/colin/Documents/Courses/multicore-522/Project/NBHashTable/build/src/CMakeFiles/test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/test.dir/depend

