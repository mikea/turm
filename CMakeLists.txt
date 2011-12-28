cmake_minimum_required(VERSION 2.6)

project(turm)
add_library(board board.cxx)
add_executable(turm turm.cxx)

set(BOOST_ROOT "$ENV{HOME}/Packages/boost_1_48_0")
find_package(Boost 1.48 REQUIRED)

include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(turm board ${Boost_LIBRARIES})

#### Tests
add_executable(runUnitTests board_test.cxx
                            turm_test.cxx)

add_test(NAME runUnitTests COMMAND runUnitTests)

### gtest setup
add_subdirectory(lib/gtest-1.6.0)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
target_link_libraries(runUnitTests gtest gtest_main board)

set(CMAKE_CXX_FLAGS "-g -Wall")

