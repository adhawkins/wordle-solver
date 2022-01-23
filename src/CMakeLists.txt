cmake_minimum_required(VERSION 3.0.0)

include(CTest)
enable_testing()

add_library(wordle-solver WordleSolver.cc WordList.cc)

add_executable(wordle-solver-bin Main.cc)
target_link_libraries(wordle-solver-bin wordle-solver)
set_target_properties(wordle-solver-bin PROPERTIES OUTPUT_NAME wordle-solver)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
