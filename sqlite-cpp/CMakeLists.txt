cmake_minimum_required(VERSION 3.0.0)
project(simplex-sqlite VERSION 1.0.0)

include(CTest)
enable_testing()

add_library(simplex-sqlite Sqlite3.cpp SqliteStatement.cpp)
target_link_libraries(simplex-sqlite simplex-toolkit sqlite)

add_executable(simplex-test main.cpp)
target_link_libraries(simplex-test simplex-sqlite)