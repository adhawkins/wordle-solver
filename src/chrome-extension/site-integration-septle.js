const septleMutationClasses = [];

class CSiteIntegrationSeptle extends CSiteIntegration {
	constructor(solverInterface) {
		super(solverInterface);

		this.observer = new MutationObserver(CSiteIntegrationSeptle.changesCallback);
		wordleMutationClasses.push({
			observer: this.observer,
			integration: this
		});

		const integration = this;

		const board = document.querySelectorAll('table.squares')[0];
		if (board) {
			const rows = board.querySelectorAll('tr');
			if (rows) {
				this.lastRow = rows[rows.length - 1];
				const integration = this;
				rows.forEach((function (row) {
					const tiles = row.querySelectorAll('td');
					integration.wordLength = tiles.length;
					integration.observer.observe(tiles[tiles.length - 1], { attributes: true });
				}));
			}
		}

		const alertBox = document.querySelectorAll('#alertBox');
		if (alertBox) {
			this.observer.observe(alertBox[0], { childList: true });
		}
	}

	sendKey(key) {
		let keyCode = 0;

		switch (key) {
			case 'Enter':
				keyCode = 13;
				break;

			case 'Backspace':
				keyCode = 8;
				break;

			default:
				keyCode = key.charCodeAt(0);
				break;
		}

		const params = {
			key: key,
			keyCode: keyCode
		};

		document.dispatchEvent(new KeyboardEvent('keydown', params));
		document.dispatchEvent(new KeyboardEvent('keyup', params));

	}

	static changesCallback(changes, observer) {
		let integration = null;
		const changesMap = new Map();

		for (const mutationClass of wordleMutationClasses) {
			if (mutationClass.observer = observer) {
				integration = mutationClass.integration;
			}
		}

		for (const change of changes) {
			switch (change.type) {
				case 'attributes':
					if (change.attributeName == "data-state") {
						const newState = change.target.getAttribute(change.attributeName);
						changesMap.set(change.target, newState);
					}

					break;

				case 'childList':
					for (const node of change.addedNodes) {
						if (node.nodeName == 'SPAN' && node.textContent == 'Not in word list!') {
							integration.solverInterface.badGuess();
						}
					}

					break;
			}
		}

		for (const [target, newState] of changesMap) {
			if (newState && newState != "tbd") {
				const parent = target.parentElement;
				const tiles = parent.querySelectorAll('td');

				let result = ' '.repeat(integration.wordLength);

				for (let count = 0; count < tiles.length; count++) {
					switch (tiles[count].getAttribute('data-state')) {
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

				const row = parent;
				integration.solverInterface.guessMarked(result, row == integration.lastRow);
			}
		}
	}
}
