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

include_directories(.)
include_directories(./util)
include_directories(./mert)

add_subdirectory(util)
add_subdirectory(mert)

add_library(mosesscorers STATIC
    moses_scorers.cpp
    $<TARGET_OBJECTS:mert>
    $<TARGET_OBJECTS:util>
)

add_executable(evaluator mert/evaluator.cpp)
set_target_properties(evaluator PROPERTIES OUTPUT_NAME evaluator)
target_compile_options(evaluator PUBLIC ${ALL_WARNINGS})
target_link_libraries(evaluator ${EXT_LIBS} mosesscorers ${EXT_LIBS} ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(evaluator PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

add_executable(teralign mert/teralign.cpp)
set_target_properties(teralign PROPERTIES OUTPUT_NAME teralign)
target_compile_options(teralign PUBLIC ${ALL_WARNINGS})
target_link_libraries(teralign ${EXT_LIBS} mosesscorers ${EXT_LIBS} ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(teralign PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")