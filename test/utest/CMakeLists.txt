cmake_minimum_required(VERSION 3.10.3)

project(UnitTests)

set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
include(CTest)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTEST REQUIRED gtest)
pkg_check_modules(GMOCK REQUIRED gmock)

link_directories(${GTEST_LIBRARY_DIRS} ${GMOCK_LIBRARY_DIRS})

enable_testing()

function(nn_utest_run_add test_suite)
    gtest_discover_tests(${test_suite} DISCOVERY_TIMEOUT 10 TEST_PREFIX ${test_suite}:)
    message("Registering test suite: ${test_suite}")
#    add_test(NAME ${test_suite} COMMAND ${test_suite})
endfunction()

add_subdirectory(tests)