include(CPack)

install(FILES
		background-script.js
		content-script.js
		manifest.json
		wordle_logo_16x16.png
		wordle_logo_32x32.png
		wordle_logo_48x48.png
		wordle_logo_128x128.png
		DESTINATION chrome-extension
		COMPONENT chrome-extension)

