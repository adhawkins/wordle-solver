class WordleLetter extends HTMLElement {
	static get observedAttributes() {
		return [
			'letter',
			'status',
			'marked',
		];
	}

	constructor() {
		super();

		this.attachShadow({ mode: 'open' });
		this.wrapper = document.createElement('div');
		this.wrapper.setAttribute('class', 'letter');

		const style = document.createElement('style');
		style.textContent = `
		.letter {
			width: 100%;
			display: inline-flex;
			justify-content: center;
			align-items: center;
			font-size: 2rem;
			line-height: 2rem;
			font-weight: bold;
			vertical-align: middle;
			box-sizing: border-box;
			color: black;
			text-transform: uppercase;
			border: 2px solid;
			user-select: none;
		}

		.letter[state='correct'] {
			background-color: #6aaa64;
			color: white;
		}

		.letter[state='inword'] {
			background-color: #c9b458;
			color: white;
		}

		.letter[state='missing'] {
			background-color: #787c7e;
			color: white;
		}`;

		this.shadowRoot.append(style, this.wrapper);
		this.updateContent(this.getAttribute('letter'));

		this.addEventListener('click', () => {
			const disabled = this.getAttribute('marked');
			if (!disabled) {
				const currentState = this.getAttribute('status')
				switch (currentState) {
					case 'missing':
						this.setAttribute('status', 'inword');
						break;

					case 'inword':
						this.setAttribute('status', 'correct');
						break;

					case null:
					case 'correct':
						this.setAttribute('status', 'missing');
						break;
				}
			}
		});
	}

	attributeChangedCallback(name, oldValue, newValue) {
		switch (name) {
			case 'letter':
				this.updateContent(newValue);
				break;

			case 'status':
				this.wrapper.setAttribute('state', `${newValue}`);

				this.dispatchEvent(new CustomEvent("LetterStatusChanged", {
					bubbles: true,
					composed: true,
				}));

				break;
		}
	}

	updateContent(letter) {
		this.wrapper.textContent = letter;
	}
}

customElements.define('wordle-letter', WordleLetter);