cmake_minimum_required(VERSION 3.0.0)

add_executable(wordle-solver-bin Main.cc)
target_link_libraries(wordle-solver-bin wordle-solver)
target_include_directories(wordle-solver-bin PRIVATE ../lib)

set_target_properties(wordle-solver-bin PROPERTIES OUTPUT_NAME wordle-solver)

install(TARGETS wordle-solver-bin
	RUNTIME
	DESTINATION bin
	COMPONENT console)