USAGE INSTRUCTIONS

0. Explanation

Native addon:

- The native module is specified in the binding.gyp file
- In specifies source files and also locations of linked libraries and headers
- It makes use of the "api" library from the main project
- To be able to keep state among calls, the api uses a C++ object.
- A "wrapper" object is used to be able to access the C++ object from JS.
- The result of the build is a single file (lester.node)
- The native module can be directly used as shown in test1.js and test2.js
- During build the binary file is copied to the specific platform folder within the publish folder.
- Also the checkpoints are copied.

Module segment-anything-model:

- With a package.json that specifies a install script in install.js
- This script retrieves another model, specific for the platform (e.g. segment-anything-model-mac).
- Tha main module also includes main.js the script that is executed when the module is required by a script. 

1. Setup

Install Node.js: https://nodejs.org/en/download/package-manager/

Install node-gyp:

npm install -g node-gyp

(LINK: https://github.com/nodejs/node-gyp)

NOTE: Some paths assume you are within the back2black repo. Somo other paths asume you have opencv in a folder at the same level of the back2black repo.

SETUP Windows

You need to install the latest version of Visual Studio including the "Desktop development with C++" workload (https://github.com/nodejs/node-gyp#on-windows)

Install Chocolatey (https://chocolatey.org/install):
	Open PowerShell with administrative privileges
	
	Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
	
	choco -?

Install Visual Studio tools:
	
	choco install python visualstudio2022-workload-vctools -y


2. Build and publish

From the root (lester-node-addon_latest):

(in MSVC previously do: fnm env --use-on-cd | Out-String | Invoke-Expression)

node-gyp rebuild

NOTE 1 : Better than configure build because that way if anything changes in a linked library the build is not going to be updated unless you change something in the addon files.

NOTE 2 : The build target is "build" and cannot be changed. It will be overwritten if you build in a different platform but the relevant results are copied to the specific folder within the publish folder. 

cd publish/segment-anything-model
(change the version in publish/segment-anything-model/package.json)
npm publish

And now for the specific platform where you are building:

cd publish/segment-anything-model-mac
(change the version in publish/segment-anything-model-mac/package.json)
npm publish

NOTE: For publishing you need previously to:
	
	- Register here: https://www.npmjs.com/signup
	- Log into https://registry.npmjs.org/ (rtous, ruben.tous@upc.edu)
	npm adduser

3. Test the C++ addon directly

node test1.js
node test2.js

4. Test the published module

In the test directory remove everything except the test.js and do:

npm install segment-anything-model
node test.js

HOWTO INSTRUCTIONS

1. Publishing a module

- Create a package.json file (root of the module)
	
	npm init //(name = "segment-anything-model")

- Create a main.js (or the name that appears in package.json)

- Log into https://registry.npmjs.org/ (rtous, ruben.tous@upc.edu)

	(register here: https://www.npmjs.com/signup)

	npm adduser

- publish

	npm publish


7. Troubleshooting

- At build: use of typeid requires -frtti: 
	"targets": [
    	{	
    		...
			"xcode_settings": {
		          "GCC_ENABLE_CPP_RTTI": "YES",
		          "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
		    },
		}
	]

- At build: error: cannot use 'throw' with exceptions disabled

- At build time: X.h missing: "include_dirs": A directory is missing within the .gyp file, in the "include_dirs" array.

- At execution time: dyld[11953]: missing symbol called: A library is missing within the .gyp file, in the "libraries" array.

6. Other relevant info

- Shared objects: Check "Wrapping C++ objects" in https://nodejs.org/api/addons.html

- Complex gyp file: 
	- https://github.com/nodejs/node-v0.x-archive/blob/f39e608c6eb11c91839ea4661caece1f89f1b12f/common.gypi
	- https://github.com/lgybetter/opencv-node-addon

- Another complex addon you can install: https://github.com/svenpaulsen/node-ts3sdk-client

- Some repos migrating from GYP to GN (Ninja)

- Path to addon: https://stackoverflow.com/questions/27465432/path-of-current-node-js-c-addon

- publish to repo: https://docs.npmjs.com/creating-node-js-modules

- Control which files you publish: 
https://medium.com/trabe/control-what-you-publish-inside-your-npm-packages-e3ec911638b8

- For multiple platforms: https://stackoverflow.com/questions/48019459/can-you-pack-multiple-native-modules-into-the-same-npm-package