updateIcon();

async function updateIcon() {
	const tab = await getCurrentTab();
	if (tab) {
		if (tab.url == "https://www.nytimes.com/games/wordle/index.html") {
			chrome.action.enable();
		} else {
			chrome.action.disable();
		}
	} else {
		chrome.action.disable();
	}
}

async function getCurrentTab() {
	const queryOptions = { active: true, currentWindow: true };
	const [tab] = await chrome.tabs.query(queryOptions);
	return tab;
}

function sendMessage(message) {
	chrome.tabs.query({ active: true, currentWindow: true }, function (tabs) {
		chrome.tabs.sendMessage(tabs[0].id, message);
	});
}

chrome.windows.onFocusChanged.addListener(async function (window) {
	updateIcon();
});

chrome.tabs.onActivated.addListener(async function (activeInfo) {
	updateIcon();
});

chrome.tabs.onUpdated.addListener(async function (tabID, changeInfo, tab) {
	updateIcon();
});

chrome.action.onClicked.addListener(function () {
	sendMessage({ "action": "buttonClicked" });
});

chrome.runtime.onMessage.addListener(function (message, sender, sendResponse) {
	switch (message.action) {
		case "complete":
			chrome.action.setBadgeText({
				text: "done",
				tabId: sender.tab.id
			});

			break;

		case "failed":
			chrome.action.setBadgeText({
				text: "fail",
				tabId: sender.tab.id
			});

			break;
	}

	return true;
});
