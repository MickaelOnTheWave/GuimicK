cmake_minimum_required(VERSION 3.5)
set(CMAKE_POLICY_DEFAULT_CMP0072 NEW)

project(ConfigurationEditiongTool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6Widgets REQUIRED)

add_definitions(-DVERSION="${APP_VERSION}")

set (projectRoot "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set (PROG_ROOT "${projectRoot}/../..")
set (TASKMANAGER_ROOT "${PROG_ROOT}/TaskManager")
set (TOOLS_ROOT "${PROG_ROOT}/Tools")

set (rootDir "src")
set (scheduleDir "${rootDir}/scheduler")
set (toolRunDir "${rootDir}/tasktoolrun")

file(GLOB rootSources "${rootDir}/*")
file(GLOB schedulerSources "${scheduleDir}/*")
file(GLOB toolRunSources "${toolRunDir}/*")
file(GLOB wakeJobSources "${rootDir}/wakejob/*")
file(GLOB winSchedulerSources "${scheduleDir}/windows/*")
file(GLOB winToolRunSources "${toolRunDir}/windows/*")

set(sources ${rootSources} ${schedulerSources} ${toolRunSources} ${wakeJobSources})

if(WIN32)
    list(APPEND sources ${winSchedulerSources} ${winToolRunSources})
endif(WIN32)

if (CLIENT_VERSION)
    add_definitions(-DNO_DEV_TOOLS -DSTANDALONE_ONLY)
endif(CLIENT_VERSION)

if(WIN32)
        add_executable(ConfigurationEditingTool ${sources} "resources/resources.qrc" "resources/ConfigurationEditingTool.rc")
else(WIN32)
        add_executable(ConfigurationEditingTool ${sources} "resources/resources.qrc")
endif(WIN32)

target_include_directories(ConfigurationEditingTool PRIVATE ${rootDir})
target_include_directories(ConfigurationEditingTool PRIVATE ${scheduleDir})
target_include_directories(ConfigurationEditingTool PRIVATE ${toolRunDir})

target_link_libraries (ConfigurationEditingTool TaskLib BasicToolsLib QtToolsLib Qt6::Widgets)

set(toolsLibPath "${TOOLS_ROOT}/ToolsLib/lib")

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    if (${CMAKE_SIZEOF_VOID_P} STREQUAL "4")
        target_link_libraries (ConfigurationEditingTool "${toolsLibPath}/ws2_32.lib")
    elseif (${CMAKE_SIZEOF_VOID_P} STREQUAL "8")
        set(editToolLibPath "${TASKMANAGER_ROOT}/ConfigurationEditingTool/lib")
        target_link_libraries (ConfigurationEditingTool "${toolsLibPath}/ws2_64.lib")
        target_link_libraries (ConfigurationEditingTool "${editToolLibPath}/taskschd_64.lib")
        target_link_libraries (ConfigurationEditingTool "${editToolLibPath}/credui_64.lib")
    endif()
endif ()

if(MSVC)
  target_compile_options(ConfigurationEditingTool PRIVATE /W4)
else(MSVC)
  target_compile_options(ConfigurationEditingTool PRIVATE -Wall -Wextra -pedantic)
endif(MSVC)
