cmake_minimum_required (VERSION 3.2)

project (ParsersLib)

file(GLOB SOURCES "../../src/*")

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../src/")

add_library(ParsersLib ${SOURCES}) 
set_target_properties(ParsersLib PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${INCLUDE_DIR}")
target_link_libraries (ParsersLib ToolsLib)
