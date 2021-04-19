cmake_minimum_required(VERSION 3.12)

project(braces_perm)

set(EX_NAME ${PROJECT_NAME}.o)

# option for debug (debug info)
set(PR_DEBUG_OPTIONS -g)

# option for release (optimization)
set(PR_RELEASE_OPTIONS -O2)

# some useful warnings
set(WARN_OPTIONS 
  -Wall -Wextra -Wunused -Wpedantic # classic
  -Wno-old-style-cast -Waggressive-loop-optimizations 
  -Weffc++
  -Wuseless-cast
  -Wunreachable-code)

add_executable(${EX_NAME} main.cc chain.cc ../matrix/matrix.cc)

target_include_directories(${EX_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${EX_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${EX_NAME} PRIVATE ../matrix)

# Specify standard
target_compile_features(${EX_NAME} PRIVATE cxx_std_20)

# Specify warning options
target_compile_options(${EX_NAME} PRIVATE ${WARN_OPTIONS})

# Specify compiler options for debug and release
target_compile_options(${EX_NAME} PRIVATE "$<$<CONFIG:DEBUG>:${PR_DEBUG_OPTIONS}>")
target_compile_options(${EX_NAME} PRIVATE "$<$<CONFIG:RELEASE>:${PR_RELEASE_OPTIONS}>")
