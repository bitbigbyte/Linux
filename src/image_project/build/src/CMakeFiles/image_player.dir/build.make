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
CMAKE_SOURCE_DIR = /home/xiaoke/linux/IMX6ULL/C_Program/image_project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/xiaoke/linux/IMX6ULL/C_Program/image_project/build

# Include any dependencies generated for this target.
include src/CMakeFiles/image_player.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/image_player.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/image_player.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/image_player.dir/flags.make

src/CMakeFiles/image_player.dir/main.c.o: src/CMakeFiles/image_player.dir/flags.make
src/CMakeFiles/image_player.dir/main.c.o: ../src/main.c
src/CMakeFiles/image_player.dir/main.c.o: src/CMakeFiles/image_player.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xiaoke/linux/IMX6ULL/C_Program/image_project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/CMakeFiles/image_player.dir/main.c.o"
	cd /home/xiaoke/linux/IMX6ULL/C_Program/image_project/build/src && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/CMakeFiles/image_player.dir/main.c.o -MF CMakeFiles/image_player.dir/main.c.o.d -o CMakeFiles/image_player.dir/main.c.o -c /home/xiaoke/linux/IMX6ULL/C_Program/image_project/src/main.c

src/CMakeFiles/image_player.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/image_player.dir/main.c.i"
	cd /home/xiaoke/linux/IMX6ULL/C_Program/image_project/build/src && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/xiaoke/linux/IMX6ULL/C_Program/image_project/src/main.c > CMakeFiles/image_player.dir/main.c.i

src/CMakeFiles/image_player.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/image_player.dir/main.c.s"
	cd /home/xiaoke/linux/IMX6ULL/C_Program/image_project/build/src && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/xiaoke/linux/IMX6ULL/C_Program/image_project/src/main.c -o CMakeFiles/image_player.dir/main.c.s

src/CMakeFiles/image_player.dir/show_bmp.c.o: src/CMakeFiles/image_player.dir/flags.make
src/CMakeFiles/image_player.dir/show_bmp.c.o: ../src/show_bmp.c
src/CMakeFiles/image_player.dir/show_bmp.c.o: src/CMakeFiles/image_player.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xiaoke/linux/IMX6ULL/C_Program/image_project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object src/CMakeFiles/image_player.dir/show_bmp.c.o"
	cd /home/xiaoke/linux/IMX6ULL/C_Program/image_project/build/src && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/CMakeFiles/image_player.dir/show_bmp.c.o -MF CMakeFiles/image_player.dir/show_bmp.c.o.d -o CMakeFiles/image_player.dir/show_bmp.c.o -c /home/xiaoke/linux/IMX6ULL/C_Program/image_project/src/show_bmp.c

src/CMakeFiles/image_player.dir/show_bmp.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/image_player.dir/show_bmp.c.i"
	cd /home/xiaoke/linux/IMX6ULL/C_Program/image_project/build/src && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/xiaoke/linux/IMX6ULL/C_Program/image_project/src/show_bmp.c > CMakeFiles/image_player.dir/show_bmp.c.i

src/CMakeFiles/image_player.dir/show_bmp.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/image_player.dir/show_bmp.c.s"
	cd /home/xiaoke/linux/IMX6ULL/C_Program/image_project/build/src && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/xiaoke/linux/IMX6ULL/C_Program/image_project/src/show_bmp.c -o CMakeFiles/image_player.dir/show_bmp.c.s

src/CMakeFiles/image_player.dir/show_jpeg.c.o: src/CMakeFiles/image_player.dir/flags.make
src/CMakeFiles/image_player.dir/show_jpeg.c.o: ../src/show_jpeg.c
src/CMakeFiles/image_player.dir/show_jpeg.c.o: src/CMakeFiles/image_player.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xiaoke/linux/IMX6ULL/C_Program/image_project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object src/CMakeFiles/image_player.dir/show_jpeg.c.o"
	cd /home/xiaoke/linux/IMX6ULL/C_Program/image_project/build/src && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/CMakeFiles/image_player.dir/show_jpeg.c.o -MF CMakeFiles/image_player.dir/show_jpeg.c.o.d -o CMakeFiles/image_player.dir/show_jpeg.c.o -c /home/xiaoke/linux/IMX6ULL/C_Program/image_project/src/show_jpeg.c

src/CMakeFiles/image_player.dir/show_jpeg.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/image_player.dir/show_jpeg.c.i"
	cd /home/xiaoke/linux/IMX6ULL/C_Program/image_project/build/src && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/xiaoke/linux/IMX6ULL/C_Program/image_project/src/show_jpeg.c > CMakeFiles/image_player.dir/show_jpeg.c.i

src/CMakeFiles/image_player.dir/show_jpeg.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/image_player.dir/show_jpeg.c.s"
	cd /home/xiaoke/linux/IMX6ULL/C_Program/image_project/build/src && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/xiaoke/linux/IMX6ULL/C_Program/image_project/src/show_jpeg.c -o CMakeFiles/image_player.dir/show_jpeg.c.s

src/CMakeFiles/image_player.dir/show_png.c.o: src/CMakeFiles/image_player.dir/flags.make
src/CMakeFiles/image_player.dir/show_png.c.o: ../src/show_png.c
src/CMakeFiles/image_player.dir/show_png.c.o: src/CMakeFiles/image_player.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xiaoke/linux/IMX6ULL/C_Program/image_project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object src/CMakeFiles/image_player.dir/show_png.c.o"
	cd /home/xiaoke/linux/IMX6ULL/C_Program/image_project/build/src && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/CMakeFiles/image_player.dir/show_png.c.o -MF CMakeFiles/image_player.dir/show_png.c.o.d -o CMakeFiles/image_player.dir/show_png.c.o -c /home/xiaoke/linux/IMX6ULL/C_Program/image_project/src/show_png.c

src/CMakeFiles/image_player.dir/show_png.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/image_player.dir/show_png.c.i"
	cd /home/xiaoke/linux/IMX6ULL/C_Program/image_project/build/src && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/xiaoke/linux/IMX6ULL/C_Program/image_project/src/show_png.c > CMakeFiles/image_player.dir/show_png.c.i

src/CMakeFiles/image_player.dir/show_png.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/image_player.dir/show_png.c.s"
	cd /home/xiaoke/linux/IMX6ULL/C_Program/image_project/build/src && arm-linux-gnueabihf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/xiaoke/linux/IMX6ULL/C_Program/image_project/src/show_png.c -o CMakeFiles/image_player.dir/show_png.c.s

# Object files for target image_player
image_player_OBJECTS = \
"CMakeFiles/image_player.dir/main.c.o" \
"CMakeFiles/image_player.dir/show_bmp.c.o" \
"CMakeFiles/image_player.dir/show_jpeg.c.o" \
"CMakeFiles/image_player.dir/show_png.c.o"

# External object files for target image_player
image_player_EXTERNAL_OBJECTS =

bin/image_player: src/CMakeFiles/image_player.dir/main.c.o
bin/image_player: src/CMakeFiles/image_player.dir/show_bmp.c.o
bin/image_player: src/CMakeFiles/image_player.dir/show_jpeg.c.o
bin/image_player: src/CMakeFiles/image_player.dir/show_png.c.o
bin/image_player: src/CMakeFiles/image_player.dir/build.make
bin/image_player: src/CMakeFiles/image_player.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/xiaoke/linux/IMX6ULL/C_Program/image_project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C executable ../bin/image_player"
	cd /home/xiaoke/linux/IMX6ULL/C_Program/image_project/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/image_player.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/image_player.dir/build: bin/image_player
.PHONY : src/CMakeFiles/image_player.dir/build

src/CMakeFiles/image_player.dir/clean:
	cd /home/xiaoke/linux/IMX6ULL/C_Program/image_project/build/src && $(CMAKE_COMMAND) -P CMakeFiles/image_player.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/image_player.dir/clean

src/CMakeFiles/image_player.dir/depend:
	cd /home/xiaoke/linux/IMX6ULL/C_Program/image_project/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xiaoke/linux/IMX6ULL/C_Program/image_project /home/xiaoke/linux/IMX6ULL/C_Program/image_project/src /home/xiaoke/linux/IMX6ULL/C_Program/image_project/build /home/xiaoke/linux/IMX6ULL/C_Program/image_project/build/src /home/xiaoke/linux/IMX6ULL/C_Program/image_project/build/src/CMakeFiles/image_player.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/image_player.dir/depend

