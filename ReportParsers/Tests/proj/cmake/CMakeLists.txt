cmake_minimum_required(VERSION 3.4)

project(ParserTest)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(projectRoot "../..")
set(taskManagerRoot "${projectRoot}/../..")
add_subdirectory ("${projectRoot}/../ParsersLib/proj/cmake" "./ParsersLib")
add_subdirectory ("${taskManagerRoot}/../Tools/ToolsLib/proj/cmake" "./ToolsLib")

file(GLOB sources "../../src/*")

add_executable(${PROJECT_NAME} ${sources})
target_link_libraries (${PROJECT_NAME} ParsersLib ToolsLib ${GTEST_LIBRARIES} pthread)

