cmake_minimum_required(VERSION 3.14)

project(ParsersTests LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Catch2 REQUIRED)

set(sources
    src/abstractoutputparsertestfixture.cpp
    src/abstractoutputparsertestfixture.h
    src/aptupgradeparsertestfixture.cpp
    src/aptupgradeparsertestfixture.h
    src/dfparsertestfixture.h
    src/diskparsertestfixture.cpp
    src/diskparsertestfixture.h
    src/filebackupparsertestfixture.cpp
    src/filebackupparsertestfixture.h
    src/aptupgradeparsertests.cpp
    src/chkdskparsertests.cpp
    src/dfparsertests.cpp
)

add_executable(ParsersTests ${sources})

target_link_libraries(ParsersTests ParsersLib Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(ParsersTests)
