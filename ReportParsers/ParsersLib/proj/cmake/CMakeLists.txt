cmake_minimum_required (VERSION 3.2)

project (ParsersLib)

set(includeDir "../../include")

file(GLOB sources "../../src/*")
file(GLOB includes "${includeDir}/*")

add_library(ParsersLib ${sources})
add_custom_target(dummyParsersLibIncludes SOURCES ${includes})

target_include_directories(ParsersLib PUBLIC ${includeDir})
target_link_libraries (ParsersLib ToolsLib)
