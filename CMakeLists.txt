cmake_minimum_required(VERSION 3.10)

project(SExxpr)

set(CMAKE_CXX_STANDARD 17)

set(SEXPR_PROJECT_NAME SExxpr)

add_library(${SEXPR_PROJECT_NAME} INTERFACE)
target_include_directories(${SEXPR_PROJECT_NAME} INTERFACE src/)

add_executable(dump-sexpr-example examples/cmd.cpp)
target_link_libraries(dump-sexpr-example ${SEXPR_PROJECT_NAME})

enable_testing()
add_subdirectory(extern/googletest)
add_subdirectory(unittests)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_subdirectory(fuzz)
endif()
