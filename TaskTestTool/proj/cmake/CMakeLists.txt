cmake_minimum_required (VERSION 3.2)

project (TaskTestTool)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt5Core REQUIRED)

set(TASKMANAGER_ROOT "../../..")
set(QTTOOLS_ROOT "../../../../")
set(TOOLS_ROOT "../../../../")

add_subdirectory ("${TOOLS_ROOT}/Tools/ToolsLib/proj/cmake" "./ToolsLib")
add_subdirectory ("${TOOLS_ROOT}/Tools/ToolsTestLib/proj/cmake" "./ToolsTestLib")
add_subdirectory ("${TASKMANAGER_ROOT}/ReportParsers/ParsersTestLib/proj/cmake" "./ParsersTestLib") 
add_subdirectory ("${TASKMANAGER_ROOT}/ReportParsers/ParsersLib/proj/cmake" "./ParsersLib")
add_subdirectory ("${TASKMANAGER_ROOT}/TaskLib/proj/cmake" "./TaskLib")
add_subdirectory ("${TASKMANAGER_ROOT}/TaskTestLib/proj/cmake" "./TaskTestLib")
add_subdirectory ("${QTTOOLS_ROOT}/QtTools/QtTools/proj/cmake" "./QtToolsLib")
add_subdirectory ("${QTTOOLS_ROOT}/QtTools/QtToolsTestLib/proj/cmake" "./QtToolsTestLib")

file(GLOB SOURCES "../../src/*")

add_executable(TaskTestTool ${SOURCES}) 
target_link_libraries (TaskTestTool ParsersTestLib TaskTestLib QtToolsTestLib ToolsTestLib Qt5::Core)

