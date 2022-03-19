class CSiteIntegrationInterface {
	guessMarked(result, lastRow) {
		throw new Error("guessMarked must be overridden");
	}

	badGuess() {
		throw new Error("badGuess must be overridden");
	}
}

class CSiteIntegration {
	wordLength = 5;
	solverInterface = null

	constructor(solverInterface) {
		this.solverInterface = solverInterface;

		if (this.constructor == CSiteIntegration) {
			throw new Error("Abstract classes can't be instantiated.");
		}
	}

	static setCharAt(str, index, chr) {
		if (index > str.length - 1) {
			return str
		};

		return str.substring(0, index) + chr + str.substring(index + 1);
	}

	sendKey(key) {
		throw new Error("sendKey must be overridden");
	}

	setNumWords(guessNum, numWords) {
		console.log("setNumWords not implemented in derived class - intentional?");
	}

	sendGuess(guess, guessNum, numWords) {
		this.setNumWords(guessNum, numWords);

		for (const c of guess) {
			this.sendKey(c);
		}

		this.sendKey('Enter');
	}

	clearGuess() {
		for (let i = 0; i < this.wordLength; i++) {
			this.sendKey('Backspace');
		}
	}
}