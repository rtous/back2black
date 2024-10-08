//from: https://github.com/svenpaulsen/node-ts3sdk-client/blob/master/api.js
const module_name = "segment-anything-model";

const fs   = require('fs');
const os   = require('os');
const path = require('path');

module.exports = require('./bin/' + getPlatform() + '/lester.node');

module.exports.getResourcePath  = getResourcePath;
module.exports.resourcePath  = getResourcePath();
module.exports.getPlatform      = getPlatform;
module.exports.initSAM      = initSAM;

function initSAM()
{
  return new module.exports.MyObject(module.exports.resourcePath+"lester.node");
}

function getPlatform()
{
  switch(os.type())
  {
    case 'Windows_NT':
      return (os.arch() == 'x64' ? 'win64' : 'win32');
    case 'Linux':
      return (os.arch() == 'x64' ? 'linux_amd64' : 'linux_x86');
    case 'Darwin':
      return 'mac';
    default:
      return 'unknown';
  }
}

function getResourcePath()
{
  var relPath = './bin/' + getPlatform();
  var absPath = path.resolve(path.join(__dirname, 'node_modules/'+module_name, relPath));

  try
  {
    if(fs.statSync(absPath).isDirectory())
    {
      return path.normalize(absPath) + path.sep;
    }
  }
  catch(err)
  {
    absPath = path.resolve(path.join(__dirname, relPath));
  }

  try
  {
    if(fs.statSync(absPath).isDirectory())
    {
      return path.normalize(absPath) + path.sep;
    }
  }
  catch(err)
  {console.log("getResourcePath() failed: "+err);}

  return relPath + path.sep;
}