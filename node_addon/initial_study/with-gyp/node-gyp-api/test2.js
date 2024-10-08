const fs = require('fs');

const addon = require('./Release/lester');
const addon_path = require.resolve('./Release/lester');

const obj = new addon.MyObject(addon_path);

fs.readFile('img.jpg', function(err, data) {
  if (err) throw err // Fail if the file can't be read.
  //data contains the image
  console.log('Image read');
  try {  
    obj.precompute_image(data)  
    let res2 = obj.compute_mask();
    let matBuffer2 = Buffer.from(res2);
    fs.writeFile('output4.jpg', matBuffer2,  "binary", function(err) { }); 
  } catch (e) {
    console.error(e);
  }  
})


