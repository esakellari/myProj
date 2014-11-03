
text = localStorage.getItem('text');
text = text.replace(/\n\r?/g, '<br />');
document.write(text);

