cmake_minimum_required (VERSION 3.2)

project (ConfigurationEditingTool)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets REQUIRED)

set (TASKMANAGER_ROOT "../../..")
set (TOOLS_ROOT "/home/mickael/Prog/Tools")

add_subdirectory ("${TASKMANAGER_ROOT}/TaskLib/proj/cmake" "./TaskLib")
add_subdirectory ("${TASKMANAGER_ROOT}/ReportParsers/ParsersLib/proj/cmake" "./ParsersLib")
add_subdirectory ("${TOOLS_ROOT}/ToolsLib/proj/cmake" "./ToolsLib") 

file(GLOB SOURCES "../../src/*")

add_executable(ConfigurationEditingTool ${SOURCES}) 
target_link_libraries (ConfigurationEditingTool TaskLib Qt5::Widgets)

