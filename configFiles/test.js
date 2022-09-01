'use strict'
const fs = require('fs');

fs.readFile('test.txt','ISO-8859-1', (err,data) => {
	console.log(data.charCodeAt(0),data.charCodeAt(1),data.charCodeAt(2),data.charCodeAt(3));
});
