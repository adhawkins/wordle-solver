var Module = {
	initialised: false,

	onRuntimeInitialized: function () {
		this.initialised = true;
	},

	locateFile: function (file, prefix) {
		return chrome.runtime.getURL(file);
	}
};

let gameKeyboard = null;

function setCharAt(str, index, chr) {
	if (index > str.length - 1) {
		return str
	};

	return str.substring(0, index) + chr + str.substring(index + 1);
}
function sendKey(key) {
	if (gameKeyboard) {
		gameKeyboard[0].dispatchEvent(new CustomEvent("game-key-press", {
			bubbles: !0,
			composed: !0,
			detail: {
				key: key
			}
		}));
	}
}

function sendGuess(guess) {
	const guessNum = solver.GuessNum();
	const numWords = solver.NumWords();

	wordCounts[guessNum - 1].textContent = numWords;

	for (const c of guess) {
		sendKey(c);
	}

	sendKey('Enter');
}

function clearGuess() {
	for (let i = 0; i < wordLength; i++) {
		sendKey('Backspace');
	}
}

function changesCallback(changes) {
	for (const change of changes) {
		if (change.attributeName == "reveal") {
			const target = change.target;
			const parent = target.parentElement;
			const tiles = parent.querySelectorAll('game-tile');

			let result = ' '.repeat(wordLength);

			for (let count = 0; count < tiles.length; count++) {
				switch (tiles[count].getAttribute('evaluation')) {
					case 'absent':
						result = setCharAt(result, count, '0');
						break;

					case 'present':
						result = setCharAt(result, count, '1');
						break;

					case 'correct':
						result = setCharAt(result, count, '2');
						break;
				}
			}

			if (result == '2'.repeat(wordLength)) {
				chrome.runtime.sendMessage({
					"action": "complete"
				});
			} else {
				const row = parent.getRootNode().host;
				if (row == lastRow) {
					chrome.runtime.sendMessage({
						"action": "failed"
					});
				} else {
					const parsedMatches = Module.CWordleSolver.ParseMatches(result);
					const nextGuess = solver.Result(parsedMatches);

					setTimeout(() => {
						sendGuess(nextGuess);
					}, 1000);
				}
			}
		}
	}
}

let lastRow = null;
let wordLength = 5;
let dictionary = "";
const wordCounts = Array();

const observer = new MutationObserver(changesCallback);

const gameApp = document.querySelectorAll('game-app');
const gameAppContent = gameApp[0].shadowRoot;

let solver = null;

chrome.runtime.onMessage.addListener(function (message, sender, sendResponse) {
	if (Module.initialised) {
		if (gameAppContent) {
			gameKeyboard = gameAppContent.querySelectorAll('game-keyboard');

			const gameRows = gameAppContent.querySelectorAll('game-row');

			wordLength = Number(gameRows[0].getAttribute('length'));
			switch (wordLength) {
				case 5:
					dictionary = "word-lists/wordle-words.txt";
					break;

				case 6:
					dictionary = "word-lists/wordle2-words.txt";
					break;

				case 7:
					dictionary = "word-lists/septle-words.txt";
					break;

				default:
					dictionary = "word-lists/words_alpha.txt";
					break;
			}

			lastRow = gameRows[gameRows.length - 1];

			gameRows.forEach((function (row) {
				const rowContent = row.shadowRoot;
				const rowDiv = rowContent.querySelectorAll('div')[0];
				rowDiv.style.gridTemplateColumns = `repeat(${wordLength + 1}, 1fr)`;
				let span = document.createElement('SPAN');
				span.textContent = ' ';
				rowDiv.appendChild(span);
				wordCounts.push(span);
				const tiles = rowContent.querySelectorAll('game-tile');
				observer.observe(tiles[tiles.length - 1], { attributes: true });

				row.addEventListener('animationend', (function (a) {
					if (a.animationName == 'Shake') {
						clearGuess();

						const nextGuess = solver.InvalidWord();

						setTimeout(() => {
							sendGuess(nextGuess);
						}, 1000);
					}
				}));
			}));
		}

		solver = new Module.CWordleSolver(dictionary, wordLength, "");
		const guess = solver.InitialGuess();

		setTimeout(() => {
			sendGuess(guess);
		}, 1000);
	}

	return true;
});

