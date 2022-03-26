cmake_minimum_required(VERSION 3.0.0)

add_executable(wordle-solver-bin Main.cc)
target_link_libraries(wordle-solver-bin wordle-solver)
target_include_directories(wordle-solver-bin PRIVATE ../lib ${CMAKE_BINARY_DIR})

if(MSVC)
	find_path(GETOPT_INCLUDE_DIR NAMES getopt.h)
	find_library(GETOPT_LIB NAMES getopt)
	target_include_directories(wordle-solver-bin PRIVATE ${GETOPT_INCLUDE_DIR})
	target_link_libraries(wordle-solver-bin shlwapi ${GETOPT_LIB})

	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/getopt.dll
		DESTINATION bin
		COMPONENT console)
endif()

set_target_properties(wordle-solver-bin PROPERTIES OUTPUT_NAME wordle-solver)

install(TARGETS wordle-solver-bin
	RUNTIME	DESTINATION bin
	COMPONENT console)

if(PANDOC_EXE)
	install(FILES ${CMAKE_BINARY_DIR}/README.html
		DESTINATION .
		COMPONENT console
	)
endif()
