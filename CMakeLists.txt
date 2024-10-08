cmake_minimum_required(VERSION 3.26)
project(automat)
set(CMAKE_CXX_FLAGS --coverage)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

include_directories(${CMAKE_SOURCE_DIR})

SET(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FETCHCONTENT_MAKEAVAILABLE(googletest)



enable_testing()

add_executable(testing
        tests/test_NFA.cpp
        tests/test_DFA.cpp
        tests/test_MDFA.cpp
        tests/test_returning_reg_exp.cpp
        tests/test_creating_from_reg_exp.cpp
)

add_library(automatic src/automatic_machine.cpp src/normolized_automatic_machine.cpp)

message("${CMAKE_SOURCE_DIR}")
target_include_directories(testing PUBLIC ${CMAKE_ROOT}/tests)
target_link_libraries(
        testing
        GTest::gtest_main
        automatic
)

include(GoogleTest)
gtest_discover_tests(testing)