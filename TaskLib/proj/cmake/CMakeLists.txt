cmake_minimum_required (VERSION 3.2)

project (TaskLib)

set(TOOLS_ROOT "/home/mickael/Prog/Tools")

add_subdirectory("${TOOLS_ROOT}/ToolsLib/proj/cmake" "./ToolsLib")
add_subdirectory("../../../ReportParsers/ParsersLib/proj/cmake" "./ParsersLib")

file(GLOB SOURCES "../../src/*")

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../src/")

add_library(TaskLib ${SOURCES}) 
set_target_properties(TaskLib PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${INCLUDE_DIR}")
target_link_libraries (TaskLib ToolsLib ParsersLib)
