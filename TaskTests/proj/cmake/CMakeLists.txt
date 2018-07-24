cmake_minimum_required (VERSION 3.2)

project (TaskSingleTestTool)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(TASKMANAGER_ROOT "../../..")
set(QTTOOLS_ROOT "../../../../QtTools")
set(TOOLS_ROOT "../../../../Tools")

add_subdirectory ("${TASKMANAGER_ROOT}/ReportParsers/ParsersTestLib/proj/cmake" "./ParsersTestLib") 
add_subdirectory ("${TASKMANAGER_ROOT}/ReportParsers/ParsersLib/proj/cmake" "./ParsersLib")
add_subdirectory ("${TASKMANAGER_ROOT}/TaskLib/proj/cmake" "./TaskLib")
add_subdirectory ("${TASKMANAGER_ROOT}/TaskTestLib/proj/cmake" "./TaskTestLib")
add_subdirectory ("${TOOLS_ROOT}/ToolsLib/proj/cmake" "./ToolsLib")
add_subdirectory ("${TOOLS_ROOT}/ToolsTestLib/proj/cmake" "./ToolsTestLib")
add_subdirectory ("${QTTOOLS_ROOT}/QtTools/proj/cmake" "./QtToolsLib")
add_subdirectory ("${QTTOOLS_ROOT}/QtToolsTestLib/proj/cmake" "./QtToolsTestLib")

file(GLOB SOURCES "../../src/*")

add_executable(TaskSingleTestTool ${SOURCES}) 
target_link_libraries (TaskSingleTestTool TaskTestLib TaskLib ParsersTestLib QtToolsTestLib ToolsTestLib)

