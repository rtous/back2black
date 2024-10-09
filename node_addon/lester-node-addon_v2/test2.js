const fs = require('fs');

const addon = require('./build/Release/lester');
//const addon_path = require.resolve('./build/Release/lester');
const addon_path = require.resolve('./publish/segment-anything-model-mac/bin/lester.node');

const obj = new addon.MyObject(addon_path);

fs.readFile('img.png', function(err, data) {
  if (err) throw err // Fail if the file can't be read.
  //data contains the image
  console.log('Image read');
  try {  
    obj.precompute_image(data)  
    let res2 = obj.compute_mask(539, 309);
    let matBuffer2 = Buffer.from(res2);
    fs.writeFile('output2.jpg', matBuffer2,  "binary", function(err) { }); 

    res2 = obj.compute_mask(500, 347);
    matBuffer2 = Buffer.from(res2);
    fs.writeFile('output3.jpg', matBuffer2,  "binary", function(err) { }); 
  } catch (e) {
    console.error(e);
  }  
})


