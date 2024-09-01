cmake_minimum_required(VERSION 3.14)

project(TaskLibTests LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Catch2 REQUIRED)

set(includeDir "include")

set(includes
    include/abstractbackupjobconfigurationtest.h
    include/abstractbackupjobtest.h
    include/abstractcopyfsbackupjobtest.h
    include/abstractjobconfigurationtest.h
    include/clientworkmanagertestfixture.h
)

set(sources
    src/abstractbackupjobconfigurationtest.cpp
    src/abstractbackupjobtest.cpp
    src/abstractcopyfsbackupjobtest.cpp
    src/abstractjobconfigurationtest.cpp
    src/clientworkmanagertests.cpp
)

add_executable(TaskLibTests ${sources} ${includes}
    src/clientworkmanagertestfixture.cpp)

target_link_libraries(TaskLibTests TaskLib QtToolsLib Catch2::Catch2WithMain)

target_include_directories(TaskLibTests PUBLIC "${includeDir}")

include(CTest)
include(Catch)
catch_discover_tests(TaskLibTests)
