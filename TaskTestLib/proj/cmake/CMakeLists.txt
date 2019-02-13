cmake_minimum_required (VERSION 3.2)

project (TaskTestLib)

find_package(Qt5Test REQUIRED)

set(projectRoot "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set(sourceDir "${projectRoot}/src/")
set(includeDir "${projectRoot}/include/")
file(GLOB sources "${sourceDir}/*")

add_library(TaskTestLib ${sources})
target_include_directories(TaskTestLib PRIVATE ${sourceDir}
                                       PUBLIC ${includeDir})
target_link_libraries (TaskTestLib TaskLib QtToolsLib ToolsLib Qt5::Test)
