cmake_minimum_required(VERSION 3.0.0)
project(sqlite3-cpp VERSION 1.0.0)

include(CTest)
enable_testing()

add_library(simplex-sqlite3 sqlite3.cpp)
target_link_libraries(simplex-sqlite3 simplex-toolkit sqlite)

add_executable(simplex-test main.cpp)
target_link_libraries(simplex-test simplex-sqlite3)