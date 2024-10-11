const fs = require('fs');
const sam = require("segment-anything-model-win64");

const segmentor = sam.initSAM();
fs.readFile('img.png', function(err, data) {
  if (err) throw err // Fail if the file can't be read.
  //data contains the image
  console.log('Image read');
  try {  
  	//precompute the image (just once)
    segmentor.precompute_image(data) 

    //compute a first mask at some point  
    let res2 = segmentor.compute_mask(539, 309);
    let matBuffer2 = Buffer.from(res2);
    fs.writeFile('out1.jpg', matBuffer2,  "binary", function(err) { }); 
  
    //compute a second mask at some other point  
    res2 = segmentor.compute_mask(500, 347);
    matBuffer2 = Buffer.from(res2);
    fs.writeFile('out2.jpg', matBuffer2,  "binary", function(err) { }); 
  } catch (e) {
    console.error(e);
  }  
})

