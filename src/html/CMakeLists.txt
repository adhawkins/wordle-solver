include(CPack)

install(FILES
		index.html
		index.css
		wordle-guess.js
		wordle-letter.js
		words-loader.js
		COMPONENT web
		DESTINATION web)
