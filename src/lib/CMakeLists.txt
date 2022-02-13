cmake_minimum_required(VERSION 3.0.0)

add_library(wordle-solver WordleSolver.cc WordList.cc)

if(EMSCRIPTEN)
	file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/null.cc "")

	set_target_properties(wordle-solver PROPERTIES LINK_FLAGS "-s NO_DYNAMIC_EXECUTION=1")

	add_custom_target(copy-words
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/word-lists ${CMAKE_CURRENT_BINARY_DIR}/word-lists
		)

	add_dependencies(wordle-solver copy-words)

	add_executable(wordle-solver-lib null.cc)

	target_link_libraries(wordle-solver-lib PUBLIC
		-Wl,--whole-archive
		wordle-solver
		-Wl,--no-whole-archive)

	set_target_properties(wordle-solver-lib PROPERTIES LINK_FLAGS "--bind --preload-file word-lists -s NO_DYNAMIC_EXECUTION=1")

	install(FILES
			${CMAKE_CURRENT_BINARY_DIR}/wordle-solver-lib.js
			${CMAKE_CURRENT_BINARY_DIR}/wordle-solver-lib.wasm
			${CMAKE_CURRENT_BINARY_DIR}/wordle-solver-lib.data
			DESTINATION .)
else()
	install(DIRECTORY word-lists DESTINATION .)
endif()
