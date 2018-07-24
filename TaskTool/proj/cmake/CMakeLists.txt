cmake_minimum_required (VERSION 3.2)

project (TaskTool)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O2")

set(TASKMANAGER_ROOT "../../..")
set(TOOLS_ROOT "../../../../Tools")

add_subdirectory ("${TASKMANAGER_ROOT}/TaskLib/proj/cmake" "./TaskLib")
add_subdirectory ("${TASKMANAGER_ROOT}/ReportParsers/ParsersLib/proj/cmake" "./ParsersLib")
add_subdirectory ("${TOOLS_ROOT}/ToolsLib/proj/cmake" "./ToolsLib")
add_subdirectory ("${TOOLS_ROOT}/NetworkToolsLib/proj/cmake" "./NetworkToolsLib") 

file(GLOB SOURCES "../../src/*")

add_executable(TaskTool ${SOURCES}) 
target_link_libraries (TaskTool TaskLib NetworkToolsLib curl)

