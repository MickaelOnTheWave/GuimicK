cmake_minimum_required (VERSION 3.2)

project (ParsersCli)

file(GLOB SOURCES "src/*")

add_executable(ParsersCli ${SOURCES})
target_link_libraries (ParsersCli ParsersLib BasicToolsLib)

