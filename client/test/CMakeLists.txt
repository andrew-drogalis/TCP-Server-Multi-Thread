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

# ==========================================
# FUNCTIONAL TEST PRODUCTION ENV
# ==========================================
# Add a testing executable
add_executable(
  functional_tests_production_scenario
  functional_test_production_env.cpp
  ${PARENT_DIR}/src/fx_market_time.cpp
  ${PARENT_DIR}/src/fx_order_management.cpp
  ${PARENT_DIR}/src/fx_trading_model.cpp
  ${PARENT_DIR}/src/fx_utilities.cpp
  ${PARENT_DIR}/src/fx_exception.cpp)

target_include_directories(functional_tests_production_scenario PRIVATE ${PARENT_DIR}/include)

target_link_libraries(functional_tests_production_scenario PRIVATE cpr::cpr ${PARENT_DIR}/lib/libhttpmockserver.a)

target_link_libraries(
  functional_tests_production_scenario
  LINK_PUBLIC
  ${Boost_LIBRARIES}
  GTest::GTest
  GTest::Main
  ${HTTPMOCKSERVER_LIBRARIES}
  ${MHD_LIBRARIES}
  gain_capital_api)

# ==========================================
# FUNCTIONAL TEST FAILURE ENV
# ==========================================
# Add a testing executable
add_executable(
  functional_tests_failure_scenario
  unit_test_utilities.cpp
  unit_test_order_management.cpp
  ${PARENT_DIR}/src/fx_market_time.cpp
  ${PARENT_DIR}/src/fx_order_management.cpp
  ${PARENT_DIR}/src/fx_trading_model.cpp
  ${PARENT_DIR}/src/fx_utilities.cpp
  ${PARENT_DIR}/src/fx_exception.cpp)

target_include_directories(functional_tests_failure_scenario PRIVATE ${PARENT_DIR}/include)

target_link_libraries(functional_tests_failure_scenario PRIVATE cpr::cpr ${PARENT_DIR}/lib/libhttpmockserver.a)

target_link_libraries(
  functional_tests_failure_scenario
  LINK_PUBLIC
  ${Boost_LIBRARIES}
  GTest::GTest
  GTest::Main
  ${HTTPMOCKSERVER_LIBRARIES}
  ${MHD_LIBRARIES}
  gain_capital_api)

# we cannot analyse results without gcov
find_program(GCOV_PATH gcov)
if(NOT GCOV_PATH)
  message(FATAL_ERROR "Code coverage analysis requires gcov!")
endif()

include(GoogleTest)
gtest_discover_tests(unit_test)
gtest_discover_tests(functional_tests_production_scenario)
gtest_discover_tests(functional_tests_failure_scenario)
