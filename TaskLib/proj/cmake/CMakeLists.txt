cmake_minimum_required (VERSION 3.2)

project (TaskLib)

file(GLOB SOURCES "../../src/*")

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../src/")

add_library(TaskLib ${SOURCES}) 
set_target_properties(TaskLib PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${INCLUDE_DIR}")
target_link_libraries (TaskLib ParsersLib)
