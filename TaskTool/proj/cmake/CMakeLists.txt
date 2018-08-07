cmake_minimum_required (VERSION 3.2)

project (TaskTool)

if(NOT USE_OLD_COMPILER)
	set(CMAKE_CXX_STANDARD 14)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
	set(CMAKE_CXX_EXTENSIONS OFF)
endif(USE_OLD_COMPILER)

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

