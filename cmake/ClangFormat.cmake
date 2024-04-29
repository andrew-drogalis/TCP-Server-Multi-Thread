function(add_clang_format_target TARGET_NAME SOURCE_DIR)
  find_program(CLANG-FORMAT_PATH clang-format REQUIRED)
  # Find all C++ source files in the directory
  file(GLOB FORMAT_SOURCES
       LIST_DIRECTORIES false
       "${SOURCE_DIR}/*.cpp"
       "${SOURCE_DIR}/src/*.cpp"
       "${SOURCE_DIR}/include/*.h"
       "${SOURCE_DIR}/include/*.hpp"
  )
  add_custom_target(${TARGET_NAME}
        COMMAND ${CLANG-FORMAT_PATH} -i ${FORMAT_SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-format on ${SOURCE_DIR} sources"
        )
  
endfunction()
