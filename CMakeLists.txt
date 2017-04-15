cmake_minimum_required(VERSION 3.5)
project(mosesscorers C CXX)

option(ENABLE_TESTS "Enable unit testing." ON)
message(STATUS "Enable unit tests: ${ENABLE_TESTS}")

set(CMAKE_CXX_FLAGS " -std=c++11 -g -O3 -Wno-unused-result -Wno-deprecated -fPIC")

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

if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif(ENABLE_TESTS)

add_library(mosesscorers STATIC
    moses_scorers.cpp
    $<TARGET_OBJECTS:mert>
    $<TARGET_OBJECTS:util>
)
