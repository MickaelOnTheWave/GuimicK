cmake_minimum_required(VERSION 3.14)

project(TaskLibTests LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Catch2 REQUIRED)

set(includeDir "include")

file(GLOB sources "src/*")
file(GLOB includes "include/*")

add_executable(TaskLibTests ${sources} ${includes})

target_link_libraries(TaskLibTests TaskLib QtToolsLib Catch2::Catch2WithMain)

target_include_directories(TaskLibTests PUBLIC "${includeDir}")

include(CTest)
include(Catch)
catch_discover_tests(TaskLibTests)
