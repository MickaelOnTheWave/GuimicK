cmake_minimum_required (VERSION 3.2)

project (ParsersTestLib)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(QTTOOLS_ROOT "/home/mickael/Prog/QtTools")

add_subdirectory ("${QTTOOLS_ROOT}/QtTools/proj/cmake" "./QtToolsLib") 
add_subdirectory ("../../../ParsersLib/proj/cmake" "./ParsersLib") 
#add_subdirectory ("../../../../../Tools/ToolsLib/proj/cmake" "./ToolsLib") 

file(GLOB SOURCES "../../src/*")

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../src/")

add_library(ParsersTestLib ${SOURCES}) 
set_target_properties(ParsersTestLib PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${INCLUDE_DIR}")
target_link_libraries (ParsersTestLib ParsersLib QtToolsLib ToolsLib)
