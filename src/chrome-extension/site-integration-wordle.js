const wordleMutationClasses = [];

class CSiteIntegrationWordle extends CSiteIntegration {
	gameKeyboard = null;
	wordCounts = Array();
	lastRow = null;

	constructor(solverInterface) {
		super(solverInterface);

		this.observer = new MutationObserver(CSiteIntegrationWordle.changesCallback);
		wordleMutationClasses.push({
			observer: this.observer,
			integration: this
		});

		const gameApp = document.querySelectorAll('game-app');
		const gameAppContent = gameApp[0].shadowRoot;
		if (gameAppContent) {
			this.gameKeyboard = gameAppContent.querySelectorAll('game-keyboard');

			const gameRows = gameAppContent.querySelectorAll('game-row');

			const wordLength = Number(gameRows[0].getAttribute('length'));
			const wordCounts = this.wordCounts;

			this.wordLength = wordLength;
			this.lastRow = gameRows[gameRows.length - 1];

			const integration = this;
			gameRows.forEach((function (row) {
				const rowContent = row.shadowRoot;
				const rowDiv = rowContent.querySelectorAll('div')[0];
				rowDiv.style.gridTemplateColumns = `repeat(${wordLength + 1}, 1fr)`;
				let span = document.createElement('SPAN');
				span.textContent = ' ';
				rowDiv.appendChild(span);
				wordCounts.push(span);
				const tiles = rowContent.querySelectorAll('game-tile');
				integration.observer.observe(tiles[tiles.length - 1], { attributes: true });

				row.addEventListener('animationend', (function (a) {
					if (a.animationName == 'Shake') {
						integration.solverInterface.badGuess();
					}
				}));
			}));
		}
	}

	sendKey(key) {
		if (this.gameKeyboard) {
			this.gameKeyboard[0].dispatchEvent(new CustomEvent("game-key-press", {
				bubbles: !0,
				composed: !0,
				detail: {
					key: key
				}
			}));
		}
	}

	setNumWords(guessNum, numWords) {
		if (this.wordCounts[guessNum - 1]) {
			this.wordCounts[guessNum - 1].textContent = numWords;
		}
	}

	static changesCallback(changes, observer) {
		let integration = null;

		for (const mutationClass of wordleMutationClasses) {
			if (mutationClass.observer = observer) {
				integration = mutationClass.integration;
			}
		}

		for (const change of changes) {
			if (change.attributeName == "reveal") {
				const target = change.target;
				const parent = target.parentElement;
				const tiles = parent.querySelectorAll('game-tile');

				let result = ' '.repeat(integration.wordLength);

				for (let count = 0; count < tiles.length; count++) {
					switch (tiles[count].getAttribute('evaluation')) {
						case 'absent':
							result = CSiteIntegration.setCharAt(result, count, '0');
							break;

						case 'present':
							result = CSiteIntegration.setCharAt(result, count, '1');
							break;

						case 'correct':
							result = CSiteIntegration.setCharAt(result, count, '2');
							break;
					}
				}

				const row = parent.getRootNode().host;
				integration.solverInterface.guessMarked(result, row == integration.lastRow);
			}
		}
	}
}