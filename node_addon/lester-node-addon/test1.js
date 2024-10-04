//import fs from 'fs';
//import path from 'path';
//import fs from 'fs';

// hello.js
const fs = require('fs');
const addon = require('./build/Release/lester');

console.log(addon.hello());

//TEST READ AND IMAGE NODE->C++
fs.readFile('img.jpg', function(err, data) {
  if (err) throw err // Fail if the file can't be read.
  //data contains the image
  console.log('Image read');
  //const bufFromString = Buffer.from('Hello World');
  cols = addon.image_javascript2C(data);
  console.log("cols="+cols);
})

//TEST C++->NODE AND WRITE
let res = addon.image_C2javascript();
let matBuffer1 = Buffer.from(res);
fs.writeFile('output2.jpg', matBuffer1,  "binary", function(err) { });
//ab = a.buf(),
//ar = new Uint8Array(ab)
