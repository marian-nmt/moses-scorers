cmake_minimum_required(VERSION 3.5)

project(moses_scorers C CXX)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

SET(CMAKE_CXX_FLAGS " -std=c++11 -g -O3 -Wno-unused-result -Wno-deprecated -fPIC")

find_package(ZLIB)
if(ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIRS})
    set(EXT_LIBS ${EXT_LIBS} ${ZLIB_LIBRARIES})
else(ZLIB_FOUND)
    message(SEND_ERROR "Cannot find zlib")
endif(ZLIB_FOUND)

find_package(Boost COMPONENTS system filesystem program_options thread)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    set(EXT_LIBS ${EXT_LIBS} ${Boost_LIBRARIES})
else(Boost_FOUND)
    message(SEND_ERROR "Cannot find Boost libraries. Terminating.")
endif(Boost_FOUND)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/util)
include_directories(${CMAKE_SOURCE_DIR}/util/double-conversion)
include_directories(${CMAKE_SOURCE_DIR}/mert)

add_subdirectory(util)
add_subdirectory(mert)

add_library(moses_scorers STATIC
    moses_scorers.cpp
    $<TARGET_OBJECTS:mert>
    $<TARGET_OBJECTS:util>
)
