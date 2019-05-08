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

set (rootDir "../../src")
set (scheduleDir "${rootDir}/scheduler")

file(GLOB rootSources "${rootDir}/*")
file(GLOB schedulerSources "${scheduleDir}/*")
file(GLOB winSchedulerSources "${scheduleDir}/windows/*")

if(WIN32)
	set(sources ${rootSources} ${schedulerSources} ${winSchedulerSources})
else(WIN32)
	set(sources ${rootSources} ${schedulerSources})
endif(WIN32)

add_executable(ConfigurationEditingTool ${sources})

target_include_directories(ConfigurationEditingTool PRIVATE ${scheduleDir})

target_link_libraries (ConfigurationEditingTool TaskLib ToolsLib QtToolsLib Qt5::Widgets)

if ("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)" OR "${CMAKE_GENERATOR}" MATCHES "Ninja")
    set(isCompilerWin64 TRUE)
endif()

if (isCompilerWin64)
    target_link_libraries (ConfigurationEditingTool "${TOOLS_ROOT}/ToolsLib/lib/ws2_64.lib")
    target_link_libraries (ConfigurationEditingTool "${TASKMANAGER_ROOT}/ConfigurationEditingTool/lib/taskschd_64.lib")
elseif(WIN32)
    target_link_libraries (ConfigurationEditingTool "${TOOLS_ROOT}/ToolsLib/lib/ws2_32.lib")
endif(isCompilerWin64)


if(MSVC)
  target_compile_options(ConfigurationEditingTool PRIVATE /W4)
else(MSVC)
  target_compile_options(ConfigurationEditingTool PRIVATE -Wall -Wextra -pedantic)
endif(MSVC)
