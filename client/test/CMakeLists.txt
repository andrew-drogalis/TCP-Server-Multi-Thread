cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(GNUInstallDirs)

find_package(GTest)

enable_testing()

cmake_path(
  GET
  CMAKE_CURRENT_SOURCE_DIR
  PARENT_PATH
  PARENT_DIR)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GTEST_INCLUDE_DIRS})

include(${PARENT_DIR}/cmake/Tests.cmake)
include(${PARENT_DIR}/cmake/Hardening.cmake)
include(${PARENT_DIR}/cmake/PreventInSourceBuilds.cmake)
include(${PARENT_DIR}/cmake/ClangFormat.cmake)

# ==========================================
# UNIT TESTS
# ==========================================
# Add a testing executable
add_executable(
  unit_test
  unit_test.cpp)


target_include_directories(unit_test PRIVATE ${PARENT_DIR}/include)

target_link_libraries(
  unit_test
  LINK_PUBLIC
  GTest::GTest
  GTest::Main)

# we cannot analyse results without gcov
find_program(GCOV_PATH gcov)
if(NOT GCOV_PATH)
  message(FATAL_ERROR "Code coverage analysis requires gcov!")
endif()

include(GoogleTest)
gtest_discover_tests(unit_test)
