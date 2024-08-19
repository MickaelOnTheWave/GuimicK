cmake_minimum_required(VERSION 3.5)

file (STRINGS "VERSION" APP_VERSION)

project(GuimmicK VERSION "${APP_VERSION}" LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(QT_DISABLE_DEPRECATED_BEFORE=0x050F00)

add_subdirectory(TaskTool)
add_subdirectory(ConfigurationEditingTool)
add_subdirectory(libs)

add_custom_target(Other SOURCES VERSION)
