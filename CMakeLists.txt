cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0072 NEW)

file (STRINGS "VERSION" APP_VERSION)

project(GuimmicK VERSION "${APP_VERSION}" LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(libs)
add_subdirectory(TaskTool)
add_subdirectory(ConfigurationEditingTool)
add_subdirectory(tests)

if (WIN32)
    add_subdirectory(TaskToolLauncher)
endif(WIN32)


set (NonDevFiles VERSION)
source_group("Non Dev Files" FILES ${NonDevFiles})
