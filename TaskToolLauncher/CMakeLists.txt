cmake_minimum_required (VERSION 3.2)

project (TaskToolLauncher)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB sources "src/*")

add_executable(${PROJECT_NAME} ${sources})

target_link_libraries (${PROJECT_NAME} BasicToolsLib)

