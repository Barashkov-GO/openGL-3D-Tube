# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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
CMAKE_COMMAND = /snap/clion/151/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/151/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/fanat/Projects/C++/mashGraf6

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/fanat/Projects/C++/mashGraf6/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/mashGraf6.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/mashGraf6.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mashGraf6.dir/flags.make

CMakeFiles/mashGraf6.dir/main.cpp.o: CMakeFiles/mashGraf6.dir/flags.make
CMakeFiles/mashGraf6.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fanat/Projects/C++/mashGraf6/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/mashGraf6.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mashGraf6.dir/main.cpp.o -c /home/fanat/Projects/C++/mashGraf6/main.cpp

CMakeFiles/mashGraf6.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mashGraf6.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fanat/Projects/C++/mashGraf6/main.cpp > CMakeFiles/mashGraf6.dir/main.cpp.i

CMakeFiles/mashGraf6.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mashGraf6.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fanat/Projects/C++/mashGraf6/main.cpp -o CMakeFiles/mashGraf6.dir/main.cpp.s

CMakeFiles/mashGraf6.dir/stb_image.cpp.o: CMakeFiles/mashGraf6.dir/flags.make
CMakeFiles/mashGraf6.dir/stb_image.cpp.o: ../stb_image.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fanat/Projects/C++/mashGraf6/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/mashGraf6.dir/stb_image.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mashGraf6.dir/stb_image.cpp.o -c /home/fanat/Projects/C++/mashGraf6/stb_image.cpp

CMakeFiles/mashGraf6.dir/stb_image.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mashGraf6.dir/stb_image.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fanat/Projects/C++/mashGraf6/stb_image.cpp > CMakeFiles/mashGraf6.dir/stb_image.cpp.i

CMakeFiles/mashGraf6.dir/stb_image.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mashGraf6.dir/stb_image.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fanat/Projects/C++/mashGraf6/stb_image.cpp -o CMakeFiles/mashGraf6.dir/stb_image.cpp.s

# Object files for target mashGraf6
mashGraf6_OBJECTS = \
"CMakeFiles/mashGraf6.dir/main.cpp.o" \
"CMakeFiles/mashGraf6.dir/stb_image.cpp.o"

# External object files for target mashGraf6
mashGraf6_EXTERNAL_OBJECTS =

mashGraf6: CMakeFiles/mashGraf6.dir/main.cpp.o
mashGraf6: CMakeFiles/mashGraf6.dir/stb_image.cpp.o
mashGraf6: CMakeFiles/mashGraf6.dir/build.make
mashGraf6: /usr/local/lib/libglfw3.a
mashGraf6: /usr/lib/x86_64-linux-gnu/libGL.so
mashGraf6: /usr/lib/x86_64-linux-gnu/librt.so
mashGraf6: /usr/lib/x86_64-linux-gnu/libm.so
mashGraf6: CMakeFiles/mashGraf6.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/fanat/Projects/C++/mashGraf6/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable mashGraf6"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mashGraf6.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mashGraf6.dir/build: mashGraf6

.PHONY : CMakeFiles/mashGraf6.dir/build

CMakeFiles/mashGraf6.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mashGraf6.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mashGraf6.dir/clean

CMakeFiles/mashGraf6.dir/depend:
	cd /home/fanat/Projects/C++/mashGraf6/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/fanat/Projects/C++/mashGraf6 /home/fanat/Projects/C++/mashGraf6 /home/fanat/Projects/C++/mashGraf6/cmake-build-debug /home/fanat/Projects/C++/mashGraf6/cmake-build-debug /home/fanat/Projects/C++/mashGraf6/cmake-build-debug/CMakeFiles/mashGraf6.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mashGraf6.dir/depend

