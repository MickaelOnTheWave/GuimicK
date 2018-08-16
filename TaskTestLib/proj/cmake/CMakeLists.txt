cmake_minimum_required (VERSION 3.2)

project (TaskTestLib)

find_package(Qt5Test REQUIRED)

file(GLOB SOURCES "../../src/*")

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../src/")

add_library(TaskTestLib ${SOURCES}) 
set_target_properties(TaskTestLib PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${INCLUDE_DIR}")
target_link_libraries (TaskTestLib TaskLib QtToolsLib ToolsLib Qt5::Test)
