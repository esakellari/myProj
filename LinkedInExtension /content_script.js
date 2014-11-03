chrome.runtime.sendMessage({method: "getText"}, function(response) {

 	var firstname;
	var myname1,myname;
	console.log(response.text);

	
	if(document.getElementById("firstName0") != null )
		firstname = document.getElementById("firstName0").innerHTML;
	else if(document.getElementById("firstName-invitee-invitation") != null)
		firstname = document.getElementById("firstName-invitee-invitation").value;
	else if(document.getElementById("firstName-iweReconnect") != null)
		firstname = document.getElementById("firstName-iweReconnect").value;

	if(document.getElementsByClassName("act-set-name-split-link") != null){
		myname1 = document.getElementsByClassName("act-set-name-split-link");	
		myname = myname1[0].innerHTML.split(" ");
	}

	if(document.getElementById("greeting-iweReconnect") != null){
		
		var currText = firstname+",\n\n"+response.text+"\n\nBest,"+myname[0];
		var userInput = document.getElementById('greeting-iweReconnect').value;

		if(userInput.localeCompare(currText) != 0){
			document.getElementById("greeting-iweReconnect").innerHTML = firstname+",\n\n"+response.text+"\n\nBest,"+myname[0];
			document.getElementById('greeting-iweReconnect').value =  firstname+",\n\n"+response.text+"\n\nBest,"+myname[0];
		}
	}

	if(document.getElementById("greeting-invitation") != null){
		
		var currText = firstname+",\n\n"+response.text+"\n\nBest,"+myname[0];
		var userInput = document.getElementById("greeting-invitation").value;

		if(userInput.localeCompare(currText) != 0){
			document.getElementById("greeting-invitation").innerHTML  = firstname+",\n\n"+response.text+"\n\nBest,"+myname[0];
			document.getElementById("greeting-invitation").value = firstname+",\n\n"+response.text+"\n\nBest,"+myname[0];
		}
	}

	if(document.getElementById("IF-reason-iweReconnect") != null)
		document.getElementById("IF-reason-iweReconnect").checked = true;
});






