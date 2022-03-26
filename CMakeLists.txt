cmake_minimum_required(VERSION 3.0.0)

project(wordle-solver VERSION 0.1.0)

find_program(PANDOC_EXE pandoc)

if(PANDOC_EXE)
	add_custom_command(OUTPUT README.html
		COMMAND ${PANDOC_EXE} --standalone ${CMAKE_CURRENT_SOURCE_DIR}/README.md -o ${CMAKE_CURRENT_BINARY_DIR}/README.html --metadata title="Wordle Solver"
		MAIN_DEPENDENCY README.md
	)
	add_custom_target(README_HTML ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/README.html)
endif()

set(CMAKE_CXX_STANDARD 11)

if(WIN32)
	include(FetchContent)
	FetchContent_Declare(vcpkg GIT_REPOSITORY https://github.com/Microsoft/vcpkg.git)
	FetchContent_MakeAvailable(vcpkg)

	include(${vcpkg_SOURCE_DIR}/scripts/buildsystems/vcpkg.cmake)
endif()

configure_file(config.h.in config.h)

add_subdirectory(src)

include(CTest)

set(CPACK_GENERATOR "ZIP")
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)

include (CPack)

cpack_add_component(chrome-extension)
cpack_add_component(web)
cpack_add_component(console)
