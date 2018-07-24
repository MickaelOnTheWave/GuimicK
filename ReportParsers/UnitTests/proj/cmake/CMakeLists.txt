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

add_executable(UnitTests ${SOURCES}) 
target_link_libraries (UnitTests ParsersTestLib)
