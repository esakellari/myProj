document.getElementById("mybutton").addEventListener("click",saveText);

function saveText() {
	var text = document.getElementById("textarea").value;

	localStorage.setItem('text', text);
	alert("Text stored!");
	
	document.getElementById("currText").innerHTML = text.replace(/\n\r?/g, '<br />');
}


