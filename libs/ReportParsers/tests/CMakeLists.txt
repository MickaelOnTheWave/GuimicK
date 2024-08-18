cmake_minimum_required(VERSION 3.14)

project(ParsersTests LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Catch2 REQUIRED)

set(sources
    src/abstractoutputparsertest.cpp
    src/abstractoutputparsertest.h
    src/aptupgradeparsertest.cpp
    src/aptupgradeparsertest.h
    src/chkdskparsertests.cpp
    src/dfparsertests.cpp
    src/diskparsertestfixture.cpp
    src/diskparsertestfixture.h
    src/filebackupparserabstracttest.cpp
    src/filebackupparserabstracttest.h
)

add_executable(ParsersTests ${sources})
target_link_libraries(ParsersTests ParsersLib Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(ParsersTests)
