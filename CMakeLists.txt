cmake_minimum_required(VERSION 3.0.0)
project(sqlite3 VERSION 1.0.0)

include(CTest)
enable_testing()

include_directories(simplex-toolkit/include)

include(CompileSqlite.cmake)
add_subdirectory(simplex-toolkit)
add_subdirectory(sqlite3-cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
