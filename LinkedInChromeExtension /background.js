chrome.browserAction.onClicked.addListener(function(tab) {

	chrome.tabs.executeScript(null, {file: "content_script.js"});

	chrome.runtime.onMessage.addListener(function(request, sender, sendResponse) {
   		if (request.method == "getText")
			sendResponse({text: localStorage.getItem('text')});
    	else
      		sendResponse({}); 
	});
	
});
