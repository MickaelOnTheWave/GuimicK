cmake_minimum_required (VERSION 3.2)

project (CommandLineTool)

set(TOOLSROOT "/home/mickael/Prog/Tools")

add_subdirectory ("../../../ParsersLib/proj/cmake" "./ParsersLib")
add_subdirectory ("${TOOLSROOT}/ToolsLib/proj/cmake" "./ToolsLib") 

file(GLOB SOURCES "../../src/*")

add_executable(CommandLineTool ${SOURCES}) 
target_link_libraries (CommandLineTool ParsersLib ToolsLib)

