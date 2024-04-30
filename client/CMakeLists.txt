cmake_minimum_required(VERSION 3.20)
project(
  Multithread-TCP-Client
  VERSION 1.0.0
  DESCRIPTION ""
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "Debug")

include(GNUInstallDirs)

# Build Executable
add_executable(
  ${PROJECT_NAME}
  client-main.cpp
  src/tcp-client.cpp
  src/tcp-client-exception.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

cmake_path(
  GET
  CMAKE_CURRENT_SOURCE_DIR
  PARENT_PATH
  PARENT_DIR)

# ------------------------------
# Build Configuration
include(${PARENT_DIR}/cmake/CompilerWarnings.cmake)
include(${PARENT_DIR}/cmake/Sanitizers.cmake)
include(${PARENT_DIR}/cmake/StaticAnalyzers.cmake)
include(${PARENT_DIR}/cmake/Tests.cmake)
include(${PARENT_DIR}/cmake/PreventInSourceBuilds.cmake)
include(${PARENT_DIR}/cmake/ClangFormat.cmake)

myproject_assure_out_of_source_builds()
myproject_set_project_warnings(${PROJECT_NAME} TRUE "X" "" "" "X")

add_clang_format_target(RUN_CLANG-FORMAT ${CMAKE_CURRENT_SOURCE_DIR})

add_compile_options(-pipe -fPIC)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  add_compile_options(-NDDEBUG)
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Debug") 
  myproject_enable_cppcheck(FALSE "X")
  #myproject_enable_clang_tidy(${PROJECT_NAME} FALSE)
  myproject_enable_include_what_you_use()
  myproject_enable_coverage(${PROJECT_NAME})
  myproject_enable_sanitizers(
    ${PROJECT_NAME}
    TRUE
    TRUE
    TRUE
    FALSE
    TRUE)
endif()