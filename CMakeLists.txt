cmake_minimum_required(VERSION 3.15)
project(Circuits VERSION 0.9)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_executable(circ circuits.cc parser/parser.cc FB_BLD/compiler.tab.cc FB_BLD/lex.yy.cc driver/driver.cc lin_alg/matrix.cc main.cc)