var Module = {
	initialised: false,

	onRuntimeInitialized: function () {
		this.initialised = true;
	}
};

onmessage = function (e) {
	function waitInitialised() {
		if (Module.initialised) {
			const wordList = new Module.CWordList(e.data.dictionary, e.data.numLetters);
			const words = wordList.Words();
			let retWords = [];
			for (let i = 0; i < words.size(); i++) {
				retWords.push(words.get(i));
			}

			postMessage(retWords);
		} else {
			setTimeout(waitInitialised, 100);
		}
	}

	waitInitialised();
}

importScripts('wordle-solver-lib.js');

