cmake_minimum_required (VERSION 3.2)

project (TaskLib)

if(USE_OLD_COMPILER)
else(USE_OLD_COMPILER)
   set(Boost_USE_MULTITHREADED ON)
   find_package(Threads REQUIRED)
   find_package(OpenSSL REQUIRED)
   find_package(Boost COMPONENTS system REQUIRED)
   find_package(CURL)

   include_directories(/usr/local/include ${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
   if (CURL_FOUND)
       include_directories(${CURL_INCLUDE_DIRS})
       add_definitions(-DHAVE_CURL)
   endif()
endif(USE_OLD_COMPILER)


set(projectRoot "../..")
set(includeDir "${projectRoot}/include")
set(sourceDir "${projectRoot}/src")

file(GLOB includes "${includeDir}/*")
file(GLOB sources "${sourceDir}/*")

add_library(TaskLib ${sources})

target_include_directories(TaskLib PUBLIC ${includeDir})
target_link_libraries (TaskLib ParsersLib ToolsLib)

if(USE_OLD_COMPILER)
else(USE_OLD_COMPILER)
   target_link_libraries(TaskLib /usr/local/lib/libTgBot.a ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES})
endif(USE_OLD_COMPILER)
