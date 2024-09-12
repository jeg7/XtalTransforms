cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

project(XtalTransforms
  VERSION 1.0
  LANGUAGES CXX
)

# Only do these if this is the main project, and not if it is included through
# add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  # Specify the C++ standard
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED True)

  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
  endif()

  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${XtalTransforms_SOURCE_DIR}/bin
  )

  # Compiler flags
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
  set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3")

  # Testing only available if this is the main app
  # Note this needs to be done in the main CMakeLists
  # since it calls enable_testing, which must be in the
  # main CMakeLists
  option(XtalTransforms_BUILD_TESTS "Enables building of tests" ON)
else()
  option(XtalTransforms_BUILD_TESTS "Enables building of tests" OFF)
endif()

add_subdirectory(${XtalTransforms_SOURCE_DIR}/src)

if(XtalTransforms_BUILD_TESTS)
  add_subdirectory(${XtalTransforms_SOURCE_DIR}/test)
endif()
