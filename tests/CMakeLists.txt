find_package(Boost COMPONENTS unit_test_framework)

add_definitions(-DBOOST_TEST_DYN_LINK)
file(GLOB TEST_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *Test.cpp)

foreach(testSrc ${TEST_SOURCES})
    get_filename_component(testName ${testSrc} NAME_WE)

    add_executable(${testName} ${testSrc})
    target_link_libraries(${testName}
        utillib mertlib
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        ${EXT_LIBS})

    set_target_properties(${testName} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)

    add_test(NAME ${testName}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing
        COMMAND ${CMAKE_BINARY_DIR}/Testing/${testName})
endforeach(testSrc)
