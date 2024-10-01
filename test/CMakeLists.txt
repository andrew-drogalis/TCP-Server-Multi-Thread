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

include(${PARENT_DIR}/cmake/CompilerWarnings.cmake)
include(${PARENT_DIR}/cmake/Tests.cmake)
include(${PARENT_DIR}/cmake/PreventInSourceBuilds.cmake)
include(${PARENT_DIR}/cmake/ClangFormat.cmake)

# ==========================================
# UNIT TESTS
# ==========================================
# Add a testing executable
add_executable(
  all_tests
  unit_test.cpp
  functional_test.cpp
  ${PARENT_DIR}/client/src/client-exception.cpp
  ${PARENT_DIR}/client/src/client.cpp
  ${PARENT_DIR}/server/src/server-exception.cpp
  ${PARENT_DIR}/server/src/server.cpp)

target_include_directories(all_tests PRIVATE ${PARENT_DIR}/client/include ${PARENT_DIR}/server/include)

target_link_libraries(
  all_tests
  LINK_PUBLIC
  GTest::GTest
  GTest::Main)

# ==========================================
myproject_assure_out_of_source_builds()
myproject_set_project_warnings("all_tests" TRUE "X" "" "" "X")
add_clang_format_target(RUN_CLANG-FORMAT ${CMAKE_CURRENT_SOURCE_DIR})

# we cannot analyse results without gcov
find_program(GCOV_PATH gcov)
if(NOT GCOV_PATH)
  message(FATAL_ERROR "Code coverage analysis requires gcov!")
endif()

include(GoogleTest)
gtest_discover_tests(all_tests)
