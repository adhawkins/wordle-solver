class CSiteIntegrationSeptle extends CSiteIntegration {
	constructor(solverInterface) {
		super(solverInterface);

		this.wordLength = 7;
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
}