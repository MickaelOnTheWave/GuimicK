cmake_minimum_required (VERSION 3.2)

project (TaskToolLauncher)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(projectRoot "${CMAKE_SOURCE_DIR}/../..")
set(toolsRoot "${projectRoot}/../../Tools/")

add_subdirectory ("${toolsRoot}/ToolsLib/proj/cmake" "./ToolsLib")

file(GLOB sources "${projectRoot}/src/*")

add_executable(${PROJECT_NAME} ${sources})

target_link_libraries (${PROJECT_NAME} ToolsLib)

