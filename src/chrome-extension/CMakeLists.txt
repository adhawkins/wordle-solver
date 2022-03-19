include(CPack)

configure_file(manifest.json.in ${CMAKE_CURRENT_SOURCE_DIR}/manifest.json)

install(FILES
		background-script.js
		content-script.js
		site-integration.js
		site-integration-wordle.js
		site-integration-septle.js
		manifest.json
		wordle_logo_16x16.png
		wordle_logo_32x32.png
		wordle_logo_48x48.png
		wordle_logo_128x128.png
		DESTINATION chrome-extension
		COMPONENT chrome-extension)

if(PANDOC_EXE)
	install(FILES ${CMAKE_BINARY_DIR}/README.html
		DESTINATION .
		COMPONENT chrome-extension
	)
endif()
