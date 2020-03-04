cmake_minimum_required (VERSION 3.2)

project (TaskTool)

if(USE_OLD_COMPILER)
    add_definitions(-DUSE_OLD_COMPILER)
else(USE_OLD_COMPILER)
	set(CMAKE_CXX_STANDARD 14)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
	set(CMAKE_CXX_EXTENSIONS OFF)
endif(USE_OLD_COMPILER)

if(MSVC)
  add_definitions("/W4 /wd4514")
else(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O2")
endif(MSVC)

set(PROJECT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set(TASKMANAGER_ROOT "${PROJECT_ROOT}/..")
set(TOOLS_ROOT "${TASKMANAGER_ROOT}/../Tools")

add_subdirectory ("${TASKMANAGER_ROOT}/TaskLib/proj/cmake" "./TaskLib")
add_subdirectory ("${TASKMANAGER_ROOT}/ReportParsers/ParsersLib/proj/cmake" "./ParsersLib")
add_subdirectory ("${TOOLS_ROOT}/ToolsLib/proj/cmake" "./ToolsLib")
if (USE_CURL_LIB)
	add_subdirectory ("${TOOLS_ROOT}/NetworkToolsLib/proj/cmake" "./NetworkToolsLib") 
endif (USE_CURL_LIB)

file(GLOB SOURCES "../../src/*")

if (USE_CURL_LIB)
	add_definitions(-DUSE_CURL_LIB)
else (USE_CURL_LIB)
	set(srcCurlLibDispatcher "${PROJECT_ROOT}/src/curllibreportdispatcher")
	list(REMOVE_ITEM SOURCES "${srcCurlLibDispatcher}.cpp" "${srcCurlLibDispatcher}.h")
endif (USE_CURL_LIB)

add_executable(TaskTool ${SOURCES}) 

if(MSVC)
else(MSVC)
  target_compile_options(TaskTool PRIVATE -Wall -Wextra -pedantic)
endif(MSVC)


file(GLOB DataFiles "${PROJECT_ROOT}/data/*")
add_custom_target(Data SOURCES ${DataFiles})

target_link_libraries (TaskTool TaskLib)
if (USE_CURL_LIB)
	target_link_libraries (TaskTool NetworkToolsLib curl)
endif (USE_CURL_LIB)

if ("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)" OR "${CMAKE_GENERATOR}" MATCHES "Ninja")
    set(isCompilerWin64 TRUE)
endif()

if (isCompilerWin64)
    target_link_libraries (TaskTool ToolsLib "${TOOLS_ROOT}/ToolsLib/lib/ws2_64.lib")
elseif (WIN32)
    target_link_libraries (TaskTool ToolsLib "${TOOLS_ROOT}/ToolsLib/lib/ws2_32.lib")
    target_link_libraries (TaskTool "-static-libgcc" "-static-libstdc++")
endif(isCompilerWin64)

### Fake Projects, only for display in IDE
add_custom_target(ProjectFiles SOURCES
	"../WorkLog.txt"
	"../ReleasePlan.txt")

add_custom_target(BuildScripts SOURCES
	"../../scripts/BuildTools.sh"
        "../../scripts/MakeLinuxGccDebug.sh"
        "../../scripts/MakeLinuxGccRelease.sh"
)
###
