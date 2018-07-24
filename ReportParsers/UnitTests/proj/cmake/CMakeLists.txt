cmake_minimum_required (VERSION 3.2)

project (UnitTests)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory ("../../../ParsersLib/proj/cmake" "./ParsersLib")
add_subdirectory ("../../../ParsersTestLib/proj/cmake" "./ParsersTestLib")
add_subdirectory ("../../../../../QtTools/QtTools/proj/cmake" "./QtToolsLib")
add_subdirectory ("../../../../../Tools/ToolsLib/proj/cmake" "./ToolsLib") 

file(GLOB SOURCES "../../src/*")

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../src/")

add_executable(UnitTests ${SOURCES}) 
set_target_properties(UnitTests PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${INCLUDE_DIR}")
target_link_libraries (UnitTests ParsersTestLib)
