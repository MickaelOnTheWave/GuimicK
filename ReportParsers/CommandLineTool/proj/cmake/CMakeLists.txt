cmake_minimum_required (VERSION 3.2)

project (CommandLineTool)

add_subdirectory ("../../../ParsersLib/proj/cmake" "./ParsersLib") 

file(GLOB SOURCES "../../src/*")

add_executable(CommandLineTool ${SOURCES}) 
target_link_libraries (CommandLineTool ParsersLib)

