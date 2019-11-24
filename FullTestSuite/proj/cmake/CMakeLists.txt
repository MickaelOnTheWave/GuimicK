cmake_minimum_required(VERSION 3.4)

project(FullTestSuite)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(projectRoot "../..")
set(taskManagerRoot "${projectRoot}/..")
set(toolsRoot "${taskManagerRoot}/../Tools")

add_subdirectory ("${taskManagerRoot}/ReportParsers/ParsersLib/proj/cmake" "./ParsersLib")
add_subdirectory ("${taskManagerRoot}/../Tools/ToolsLib/proj/cmake" "./ToolsLib")

file(GLOB parsersTests "${taskManagerRoot}/ReportParsers/Tests/src/tests/*")
file(GLOB toolsTests "${toolsRoot}/ToolsTest/src/tests/*")

set(sources ${parsersTests} ${toolsTests} "${projectRoot}/src/main.cpp")

add_executable(${PROJECT_NAME} ${sources})
target_link_libraries (${PROJECT_NAME} ParsersLib ToolsLib ${GTEST_LIBRARIES} pthread)

