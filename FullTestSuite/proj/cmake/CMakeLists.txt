cmake_minimum_required(VERSION 3.4)

project(FullTestSuite)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(projectRoot "../..")
set(taskManagerRoot "${projectRoot}/..")
set(toolsRoot "${taskManagerRoot}/../Tools")

add_subdirectory ("${taskManagerRoot}/ReportParsers/ParsersLib/proj/cmake" "./ParsersLib")
add_subdirectory ("${toolsRoot}/ToolsLib/proj/cmake" "./ToolsLib")

set(toolsTestsDir "${toolsRoot}/ToolsTest")
file(GLOB parsersTests "${taskManagerRoot}/ReportParsers/Tests/src/tests/*")
file(GLOB toolsTests "${toolsTestsDir}/src/tests/*")

set(sources ${parsersTests} ${toolsTests} "${projectRoot}/src/main.cpp")

add_executable(${PROJECT_NAME} ${sources})
target_link_libraries (${PROJECT_NAME} ParsersLib ToolsLib ${GTEST_LIBRARIES} pthread)


add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${toolsTestsDir}/data/ $<TARGET_FILE_DIR:${PROJECT_NAME}>)
