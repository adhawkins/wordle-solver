var Module = {
	initialised: false,

	onRuntimeInitialized: function () {
		this.initialised = true;
	},

	locateFile: function (file, prefix) {
		return chrome.runtime.getURL(file);
	}
};

class CWordleSolver {
	integration = null;
	solver = null;

	constructor() {
		let dictionary = "";

		if (window.septle) {
			this.integration = new CSiteIntegrationSeptle(this);
		} else {
			this.integration = new CSiteIntegrationWordle(this);
		}

		switch (this.integration.wordLength) {
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

		this.solver = new Module.CWordleSolver(dictionary, this.integration.wordLength, "");
		this.sendGuess(this.solver.InitialGuess());
	}

	guessMarked(result, lastRow) {
		if (result == '2'.repeat(this.integration.wordLength)) {
			chrome.runtime.sendMessage({
				"action": "complete"
			});
		} else {
			if (lastRow) {
				chrome.runtime.sendMessage({
					"action": "failed"
				});
			} else {
				const parsedMatches = Module.CWordleSolver.ParseMatches(result);
				this.sendGuess(this.solver.Result(parsedMatches));
			}
		}
	}

	badGuess() {
		this.integration.clearGuess();
		this.sendGuess(this.solver.InvalidWord());
	}

	sendGuess(guess) {
		setTimeout(() => {
			const guessNum = this.solver.GuessNum();
			const numWords = this.solver.NumWords();

			this.integration.sendGuess(guess, guessNum, numWords);
		}, 1000);
	}
}


chrome.runtime.onMessage.addListener(function (message, sender, sendResponse) {
	if (Module.initialised) {
		wordleSolver = new CWordleSolver();
	}

	return true;
});

