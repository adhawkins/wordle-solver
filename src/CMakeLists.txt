cmake_minimum_required(VERSION 3.0.0)

#project(wordle-solver)

if(EMSCRIPTEN)
	SET(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/js-install CACHE PATH ADH FORCE)
endif()

add_subdirectory(lib)

if(NOT EMSCRIPTEN)
	add_subdirectory(console)
endif()


