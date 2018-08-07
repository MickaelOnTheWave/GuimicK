cmake_minimum_required (VERSION 3.2)

project (TaskTool)

if(NOT USE_OLD_COMPILER)
	set(CMAKE_CXX_STANDARD 14)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
	set(CMAKE_CXX_EXTENSIONS OFF)
endif(NOT USE_OLD_COMPILER)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O2")

set(TASKMANAGER_ROOT "../../..")
set(TOOLS_ROOT "../../../../Tools")

add_subdirectory ("${TASKMANAGER_ROOT}/TaskLib/proj/cmake" "./TaskLib")
add_subdirectory ("${TASKMANAGER_ROOT}/ReportParsers/ParsersLib/proj/cmake" "./ParsersLib")
add_subdirectory ("${TOOLS_ROOT}/ToolsLib/proj/cmake" "./ToolsLib")
add_subdirectory ("${TOOLS_ROOT}/NetworkToolsLib/proj/cmake" "./NetworkToolsLib") 

file(GLOB SOURCES "../../src/*")

if (USE_CURL_LIB)
	add_definitions(-DUSE_CURL_LIB)
else (USE_CURL_LIB)
	set(srcCurlLibDispatcher "${CMAKE_CURRENT_SOURCE_DIR}/../../src/curllibreportdispatcher")
	list(REMOVE_ITEM SOURCES "${srcCurlLibDispatcher}.cpp" "${srcCurlLibDispatcher}.h")
endif (USE_CURL_LIB)

add_executable(TaskTool ${SOURCES}) 

if (USE_CURL_LIB)
	target_link_libraries (TaskTool TaskLib NetworkToolsLib curl)
else (USE_CURL_LIB)	
	target_link_libraries (TaskTool TaskLib)
endif (USE_CURL_LIB)

