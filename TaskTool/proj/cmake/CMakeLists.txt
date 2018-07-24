cmake_minimum_required (VERSION 3.2)

project (TaskTool)

add_subdirectory ("../../../TaskLib/proj/cmake" "./TaskLib")
add_subdirectory ("../../../../Tools/NetworkToolsLib/proj/cmake" "./NetworkToolsLib") 

file(GLOB SOURCES "../../src/*")

add_executable(TaskTool ${SOURCES}) 
target_link_libraries (TaskTool TaskLib NetworkToolsLib curl)

