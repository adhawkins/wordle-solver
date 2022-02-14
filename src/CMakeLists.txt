include(CTest)

add_subdirectory(lib)

if(EMSCRIPTEN)
	add_subdirectory(html)
	add_subdirectory(chrome-extension)
else()
	add_subdirectory(console)
endif()


