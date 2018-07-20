cmake_minimum_required (VERSION 3.2)

project (ConfigurationEditingTool)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets REQUIRED)

add_subdirectory ("../../../TaskLib/proj/cmake" "./ParsersLib") 

file(GLOB SOURCES "../../src/*")

add_executable(ConfigurationEditingTool ${SOURCES}) 
target_link_libraries (ConfigurationEditingTool TaskLib ParsersLib ToolsLib Qt5::Widgets)

