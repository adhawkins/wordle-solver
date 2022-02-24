cmake_minimum_required(VERSION 3.0.0)

project(wordle-solver VERSION 0.1.0)

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
