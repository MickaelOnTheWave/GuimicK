cmake_minimum_required (VERSION 3.2)

project (ParsersLib)

file(GLOB sources "src/*")
file(GLOB includes "include/*")

add_library(ParsersLib ${sources})

target_include_directories(ParsersLib PUBLIC "include")
target_link_libraries (ParsersLib BasicToolsLib)
