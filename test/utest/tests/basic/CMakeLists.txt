set(CMAKE_BUILD_TYPE Debug)

include(GoogleTest)

set(EXEC_NAME Basic_tests)

set(DEPS_INC "../../build/libs/include/")

set(TT_INC "../../../../include/")

set(TT_PLATFORM_INC "../../../../include/platform_contracts")

set(TT_SRC "../../../../src/")

set(UTESTS_ROOT "../../")

include_directories(${TT_PLATFORM_INC})
include_directories(${TT_INC})
include_directories(${DEPS_INC})

set(TEST_SOURCES    basic_tests_main.cpp
	basic_tests.cpp)

set(TT_SOURCES ${TT_SRC}/ticktoucan.cpp)

set(FAKE_SOURCES ${UTESTS_ROOT}/fakes/ticktoucan_platform.cpp)

add_executable(${EXEC_NAME}	${FAKE_SOURCES} ${TEST_SOURCES}	${TT_SOURCES})

target_link_libraries(${EXEC_NAME} ${GMOCK_LINK_LIBRARIES} ${GMOCK_LINK_LIBRARIES})

nn_utest_run_add(${EXEC_NAME})


