cmake_minimum_required(VERSION 3.12)

project(perm)

set(MUL_ORDER ${PROJECT_NAME}_order.o) # just print order
set(MULTIPLY ${PROJECT_NAME}_multiply.o) # perform nultiply & and print measured time

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

add_executable(${MUL_ORDER} main.cc chain.cc ../matrix/matrix.cc)
add_executable(${MULTIPLY} main.cc chain.cc ../matrix/matrix.cc)
  
set(TARGETS ${MUL_ORDER} ${MULTIPLY})

foreach(TAR ${TARGETS})
  target_include_directories(${TAR} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
  target_include_directories(${TAR} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  target_include_directories(${TAR} PRIVATE ../matrix)

  # Specify standard
  target_compile_features(${TAR} PRIVATE cxx_std_20)

  # Specify warning options
  target_compile_options(${TAR} PRIVATE ${WARN_OPTIONS})

  # Specify compiler options for debug and release
  target_compile_options(${TAR} PRIVATE "$<$<CONFIG:DEBUG>:${PR_DEBUG_OPTIONS}>")
  target_compile_options(${TAR} PRIVATE "$<$<CONFIG:RELEASE>:${PR_RELEASE_OPTIONS}>")
endforeach()

target_compile_definitions(${MUL_ORDER} PRIVATE ORDER=1)
target_compile_definitions(${MULTIPLY} PRIVATE ORDER=0)

