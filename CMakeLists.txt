# CMakeList.txt : CMake project for ElfFileParser, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

add_definitions(-D_PRINT_PROGRAM_HEADER_LIST_)

add_definitions(-D_PRINT_SECTION_HEADER_LIST_)

add_definitions(-D_PRINT_STRING_TABLE_)

add_definitions(-D_PRINT_SYMBOL_TABLE_)

project ("ElfFileParser")

# Add source to this project's executable.
add_executable (
  ElfFileParser 
  "ElfFileParser.cpp" 
  "ElfParser.cpp"
  "util/Printer.cpp"    
 )

# TODO: Add tests and install targets if needed.
