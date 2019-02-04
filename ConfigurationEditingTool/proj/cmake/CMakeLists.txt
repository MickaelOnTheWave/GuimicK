cmake_minimum_required (VERSION 3.2)

project (ConfigurationEditingTool)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets REQUIRED)

set (PROG_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../..")
set (TASKMANAGER_ROOT "${PROG_ROOT}/TaskManager")
set (TOOLS_ROOT "${PROG_ROOT}/Tools")

add_subdirectory ("${TASKMANAGER_ROOT}/TaskLib/proj/cmake" "./TaskLib")
add_subdirectory ("${TASKMANAGER_ROOT}/ReportParsers/ParsersLib/proj/cmake" "./ParsersLib")
add_subdirectory ("${TOOLS_ROOT}/ToolsLib/proj/cmake" "./ToolsLib") 
add_subdirectory ("${PROG_ROOT}/QtTools/QtTools/proj/cmake" "./QtTools")

file(GLOB SOURCES "../../src/*")

if(WIN32)
    set(GUI_TYPE WIN32)
endif(WIN32)

add_executable(ConfigurationEditingTool ${GUI_TYPE} ${SOURCES})

target_link_libraries (ConfigurationEditingTool TaskLib ToolsLib QtToolsLib Qt5::Widgets)
if ("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
	target_link_libraries (ConfigurationEditingTool "${TOOLS_ROOT}/ToolsLib/lib/ws2_64.lib")
elseif(WIN32)
	target_link_libraries (ConfigurationEditingTool "${TOOLS_ROOT}/ToolsLib/lib/ws2_32.lib")
endif("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
