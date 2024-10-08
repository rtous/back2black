/*
  The execution of this script is specified within install... in the package.json
  This module just installs the specific platform module. 
*/

const os   = require('os');
const exec = require('node:child_process').exec;;

switch(getPlatform())
{
  case 'win64':
    exec('npm install segment-anything-model-win64', (err, stdout, stderr) => {
         console.log("Error installing segment-anything-model-win64: "+err);
    });
    break;
  case 'win32':
    exec('npm install segment-anything-model-win32', (err, stdout, stderr) => {
         console.log("Error installing segment-anything-model-win32: "+err);
    });
    break;
  case 'linux_amd64':
    exec('npm install segment-anything-model-linux_amd64', (err, stdout, stderr) => {
         console.log("Error installing segment-anything-model-linux_amd64: "+err);
    });
    break;
  case 'linux_x86':
    exec('npm install segment-anything-model-linux_x86', (err, stdout, stderr) => {
         console.log("Error installing segment-anything-model-linux_x86: "+err);
    });
    break;
  case 'mac':
    exec('npm install segment-anything-model-mac', (err, stdout, stderr) => {
         console.log("Error installing segment-anything-model-mac: "+err);
    });
    break;
  default:
    console.log("Error, unknown platform detected: "+getPlatform());
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
