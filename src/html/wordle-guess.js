class WordleGuess extends HTMLElement {
	static get observedAttributes() {
		return [
			'length',
			'guess',
			'marked',
			'initial-mark'
		];
	}

	constructor() {
		super();

		this.attachShadow({ mode: 'open' });
		this.wrapper = document.createElement('div');
		this.wrapper.setAttribute('class', 'row');

		const style = document.createElement('style');
		style.textContent = `
			:host {
				display: block;
			}

			.row {
				display: grid;
				grid-template-columns: repeat(5, 1fr);
				grid-gap: 5px;
			}
		`;

		this.shadowRoot.append(style, this.wrapper);
		this.updateContent(this.getAttribute('length'), this.getAttribute('guess'));
	}

	updateContent(length, guess) {
		this.wrapper.innerText = "";

		for (let count = 0; count < length; count++) {
			const tile = document.createElement('wordle-letter');
			let letter = '?';

			if (guess && guess.length > count) {
				letter = guess[count];
			}

			tile.setAttribute('offset', count);
			tile.setAttribute('letter', letter);
			this.wrapper.append(tile);
			tile.addEventListener("LetterStatusChanged", (event) => {
				this.statusUpdate();
			});

			if (letter == '?') {
				tile.setAttribute('disabled', 'true');
			}
		}

		this.markButton = document.createElement('button');
		this.markButton.innerHTML = 'Mark';
		this.markButton.setAttribute('disabled', 'true');

		this.markButton.addEventListener('click', () => {
			this.setAttribute('marked', 'true');
			this.dispatchEvent(new CustomEvent("GuessMarked", {
				bubbles: true,
				composed: true,
			}));
		});

		this.wrapper.append(this.markButton);

		this.badWordButton = document.createElement('button');
		this.badWordButton.innerHTML = 'Invalid Word';

		this.badWordButton.addEventListener('click', () => {
			this.dispatchEvent(new CustomEvent("BadWord", {
				bubbles: true,
				composed: true,
			}));
		});

		this.wrapper.append(this.badWordButton);

		this.wrapper.style['grid-template-columns'] = `repeat(${length}, 1fr) auto auto`;
	}

	statusToNum(status) {
		switch (status) {
			case null:
				return ' ';

			case 'missing':
				return 0;

			case 'inword':
				return 1;

			case 'correct':
				return 2;
		}
	}

	statusUpdate() {
		let disabled = this.getAttribute('marked');
		let mark = '';

		this.badWordButton.disabled = disabled;

		for (let i = 0; i < this.wrapper.children.length; i++) {
			const tagName = this.wrapper.children[i].tagName;
			if (tagName == 'WORDLE-LETTER') {
				const status = this.wrapper.children[i].getAttribute('status');
				if (status == null) {
					disabled = true;
				}

				mark += this.statusToNum(status);
			}
		}

		this.markButton.disabled = disabled;
		this.setAttribute('mark', mark);
	}

	updateMarked(marked) {
		for (let i = 0; i < this.wrapper.children.length; i++) {
			const tagName = this.wrapper.children[i].tagName;
			if (tagName == 'WORDLE-LETTER') {
				this.wrapper.children[i].setAttribute('marked', marked);
			}
		}
	}

	setLetterStatus(offset, status) {
		const letter = this.wrapper.querySelectorAll(`wordle-letter[offset='${offset}']`);
		if (letter && letter[0]) {
			letter[0].setAttribute('status', status);
		}
	}

	updateMarks(marks) {
		if (marks && marks.length == this.getAttribute('length')) {
			for (let count = 0; count < marks.length; count++) {
				switch (marks[count]) {
					case '0':
						this.setLetterStatus(count, 'missing');
						break;

					case '1':
						this.setLetterStatus(count, 'inword');
						break;

					case '2':
						this.setLetterStatus(count, 'correct');
						break;

				}
			}
		}
	}
	attributeChangedCallback(name, oldValue, newValue) {
		switch (name) {
			case 'length':
			case 'guess':
				this.updateContent(this.getAttribute('length'), this.getAttribute('guess'));
				this.updateMarks(this.getAttribute('initial-mark'));
				break;

			case 'marked':
				this.updateMarked(newValue);
				this.statusUpdate();
				break;

			case 'initial-mark':
				this.updateMarks(newValue);
				break;
		}
	}
}

customElements.define('wordle-guess', WordleGuess);