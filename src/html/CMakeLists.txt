include(CPack)

configure_file(index.html.in ${CMAKE_CURRENT_SOURCE_DIR}/index.html @ONLY)

install(FILES
		index.html
		index.css
		wordle-guess.js
		wordle-letter.js
		words-loader.js
		COMPONENT web
		DESTINATION web)

if(PANDOC_EXE)
	install(FILES ${CMAKE_BINARY_DIR}/README.html
		DESTINATION .
		COMPONENT web
	)
endif()
