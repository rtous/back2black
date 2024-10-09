const sam = require('./main.js');


console.log(sam.getResourcePath());

//For release (npm install "segment-anything-model") use:
//const sam = require("segment-anything-model");

//const obj = new sam.MyObject("/Users/rtous/dev/back2black/node_addon/test/node_modules/segment-anything-model/build/Release/lester.node");
const segmentor = sam.initSAM();
