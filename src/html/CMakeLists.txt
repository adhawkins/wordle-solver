include(CPack)

cpack_add_component(web)

install(FILES
		index.html
		index.css
		wordle-guess.js
		wordle-letter.js
		words-loader.js
		COMPONENT web
		DESTINATION web)
