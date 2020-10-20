# CMakeList.txt : CMake project for ElfFileParser, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("ElfFileParser")

# Add source to this project's executable.
add_executable (
  ElfFileParser 
  "ElfFileParser.cpp" 
  "ElfParser.cpp"
 "type/elf.h")

# TODO: Add tests and install targets if needed.
