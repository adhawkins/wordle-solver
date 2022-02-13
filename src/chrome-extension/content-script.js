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
	for (const c of guess) {
		sendKey(c);
	}

	sendKey('Enter');
}

function clearGuess() {
	for (let i = 0; i < 5; i++) {
		sendKey('Backspace');
	}
}

function changesCallback(changes) {
	for (const change of changes) {
		if (change.attributeName == "reveal") {
			const target = change.target;
			const parent = target.parentElement;
			const tiles = parent.querySelectorAll('game-tile');

			let result = '     ';

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

			if (result == '22222') {
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

const observer = new MutationObserver(changesCallback);

const gameApp = document.querySelectorAll('game-app');
const gameAppContent = gameApp[0].shadowRoot;

if (gameAppContent) {
	gameKeyboard = gameAppContent.querySelectorAll('game-keyboard');

	const gameRows = gameAppContent.querySelectorAll('game-row');
	lastRow = gameRows[gameRows.length - 1];

	gameRows.forEach((function (row) {
		const rowContent = row.shadowRoot;
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

let solver = null;

chrome.runtime.onMessage.addListener(function (message, sender, sendResponse) {
	if (Module.initialised) {
		solver = new Module.CWordleSolver("word-lists/wordle-words.txt", 5, "");
		const guess = solver.InitialGuess();

		setTimeout(() => {
			sendGuess(guess);
		}, 1000);
	}
});

