include(CTest)

add_library(wordle-solver WordleSolver.cc WordList.cc)

if(EMSCRIPTEN)
	file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/null.cc "")

	set_target_properties(wordle-solver PROPERTIES LINK_FLAGS "-s NO_DYNAMIC_EXECUTION=1")

	add_executable(wordle-solver-lib null.cc)

	target_link_libraries(wordle-solver-lib PUBLIC
		-Wl,--whole-archive
		wordle-solver
		-Wl,--no-whole-archive)

	set_target_properties(wordle-solver-lib PROPERTIES LINK_FLAGS "--bind --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/word-lists@/word-lists -s NO_DYNAMIC_EXECUTION=1")

	FILE(GLOB word-lists ${CMAKE_CURRENT_SOURCE_DIR}/word-lists/*)
	set_target_properties(wordle-solver-lib PROPERTIES LINK_DEPENDS "${word-lists}")

	install(FILES
			${CMAKE_CURRENT_BINARY_DIR}/wordle-solver-lib.js
			${CMAKE_CURRENT_BINARY_DIR}/wordle-solver-lib.wasm
			${CMAKE_CURRENT_BINARY_DIR}/wordle-solver-lib.data
			COMPONENT web
			DESTINATION web)

	install(FILES
			${CMAKE_CURRENT_BINARY_DIR}/wordle-solver-lib.js
			${CMAKE_CURRENT_BINARY_DIR}/wordle-solver-lib.wasm
			${CMAKE_CURRENT_BINARY_DIR}/wordle-solver-lib.data
			COMPONENT chrome-extension
			DESTINATION chrome-extension)
else()
	add_executable(TestSolver TestSolver.cc)
	target_link_libraries(TestSolver PUBLIC wordle-solver)

	FILE(READ "${CMAKE_CURRENT_SOURCE_DIR}/word-lists/wordle-words.txt" words)
	STRING(REGEX REPLACE "\n" ";" words "${words}")

	list(SUBLIST words 0 20 start_words)
	list(SUBLIST words 30 20 correct_words)

	foreach(start ${start_words})
		foreach(correct ${correct_words})
			add_test(NAME test-${start}-${correct}
				COMMAND TestSolver ${CMAKE_CURRENT_SOURCE_DIR}/word-lists/wordle-words.txt 5 ${start} ${correct})
		endforeach()
	endforeach()

	install(DIRECTORY word-lists
		COMPONENT console
		DESTINATION .)
endif()
