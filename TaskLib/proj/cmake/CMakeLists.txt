cmake_minimum_required (VERSION 3.2)

project (TaskLib)

set(projectRoot "../..")
set(includeDir "${projectRoot}/include")

file(GLOB sources "${projectRoot}/src/*")
file(GLOB includes "${includeDir}/*")

add_library(TaskLib ${sources})

target_include_directories(TaskLib PUBLIC ${includeDir})
target_link_libraries (TaskLib ParsersLib ToolsLib)
