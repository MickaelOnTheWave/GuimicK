cmake_minimum_required(VERSION 3.5)

project(TaskTool LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "src/*")

add_executable(TaskTool ${SOURCES})

target_link_libraries(TaskTool TaskLib ParsersLib BasicToolsLib)



