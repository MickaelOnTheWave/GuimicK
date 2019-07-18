cmake_minimum_required (VERSION 3.2)

project (ConfigurationEditingTool)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets REQUIRED)

set (projectRoot "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set (PROG_ROOT "${projectRoot}/../..")
set (TASKMANAGER_ROOT "${PROG_ROOT}/TaskManager")
set (TOOLS_ROOT "${PROG_ROOT}/Tools")

add_subdirectory ("${TASKMANAGER_ROOT}/TaskLib/proj/cmake" "./TaskLib")
add_subdirectory ("${TASKMANAGER_ROOT}/ReportParsers/ParsersLib/proj/cmake" "./ParsersLib")
add_subdirectory ("${TOOLS_ROOT}/ToolsLib/proj/cmake" "./ToolsLib") 
add_subdirectory ("${PROG_ROOT}/QtTools/QtTools/proj/cmake" "./QtTools")

set (rootDir "${projectRoot}/src")
set (scheduleDir "${rootDir}/scheduler")

file(GLOB rootSources "${rootDir}/*")
file(GLOB schedulerSources "${scheduleDir}/*")
file(GLOB winSchedulerSources "${scheduleDir}/windows/*")

file(GLOB wakeJobSources "${rootDir}/wakejob/*")

set(sources ${rootSources} ${schedulerSources} ${wakeJobSources})

if(WIN32)
        list(APPEND sources ${winSchedulerSources})
endif(WIN32)

add_executable(ConfigurationEditingTool ${sources} "${projectRoot}/resources/resources.qrc")

target_include_directories(ConfigurationEditingTool PRIVATE ${rootDir})
target_include_directories(ConfigurationEditingTool PRIVATE ${scheduleDir})

target_link_libraries (ConfigurationEditingTool TaskLib ToolsLib QtToolsLib Qt5::Widgets)

if ("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)" OR "${CMAKE_GENERATOR}" MATCHES "Ninja")
    set(isCompilerWin64 TRUE)
endif()

set(toolsLibPath "${TOOLS_ROOT}/ToolsLib/lib")
if (isCompilerWin64)
    set(editToolLibPath "${TASKMANAGER_ROOT}/ConfigurationEditingTool/lib")
    target_link_libraries (ConfigurationEditingTool "${toolsLibPath}/ws2_64.lib")
    target_link_libraries (ConfigurationEditingTool "${editToolLibPath}/taskschd_64.lib")
    target_link_libraries (ConfigurationEditingTool "${editToolLibPath}/credui_64.lib")
elseif(WIN32)
    target_link_libraries (ConfigurationEditingTool "${toolsLibPath}/ws2_32.lib")
endif(isCompilerWin64)


if(MSVC)
  target_compile_options(ConfigurationEditingTool PRIVATE /W4)
else(MSVC)
  target_compile_options(ConfigurationEditingTool PRIVATE -Wall -Wextra -pedantic)
endif(MSVC)
