cmake_minimum_required (VERSION 3.2)

project (TaskLib)

set(includeDir "../../include")

file(GLOB sources "../../src/*")
file(GLOB includes "${includeDir}/*")

add_library(TaskLib ${sources})
add_custom_target(dummyTaskLibIncludes SOURCES ${includes})

target_include_directories(TaskLib PUBLIC ${includeDir})
target_link_libraries (TaskLib ParsersLib ToolsLib)
