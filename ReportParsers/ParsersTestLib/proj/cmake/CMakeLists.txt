cmake_minimum_required (VERSION 3.2)

project (ParsersTestLib)

file(GLOB SOURCES "../../src/*")

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../src/")

add_library(ParsersTestLib ${SOURCES}) 
set_target_properties(ParsersTestLib PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${INCLUDE_DIR}")
target_link_libraries (ParsersTestLib ParsersLib QtToolsLib)
