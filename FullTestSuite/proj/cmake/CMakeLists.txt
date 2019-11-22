cmake_minimum_required(VERSION 3.4)

project(FullTestSuite)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(projectRoot "../..")
set(taskManagerRoot "${projectRoot}/..")

add_subdirectory ("${taskManagerRoot}/ReportParsers/ParsersLib/proj/cmake" "./ParsersLib")
add_subdirectory ("${taskManagerRoot}/../Tools/ToolsLib/proj/cmake" "./ToolsLib")

file(GLOB parsersTests "${taskManagerRoot}/ReportParsers/Tests/src/tests/*")

set(sources ${parsersTests} "${projectRoot}/src/main.cpp")

add_executable(${PROJECT_NAME} ${sources})
target_link_libraries (${PROJECT_NAME} ParsersLib ToolsLib ${GTEST_LIBRARIES} pthread)

