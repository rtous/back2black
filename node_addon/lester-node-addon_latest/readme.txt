1. Setup

LINK: https://github.com/nodejs/node-gyp

npm install -g node-gyp

2. Build

NOTE: The build target is "build" and cannot be changed. After a successful build copy the build folder to, e.g., build_mac. 

node-gyp configure build

WARNING: If anything changes in a linked library the build is not going to be updated unless you change something in the addon files. Alternativelly you can do:

node-gyp rebuild

3. Direct test

node test1.js
node test2.js

4. Release

- Copy the Release folder from the add-on


5. Publish

- Create a package.json file (root of the module)
	
	npm init //(name = "segment-anything-model")

- Create a main.js (or the name that appears in package.json)

- Log into https://registry.npmjs.org/ (rtous, ruben.tous@upc.edu)

	(register here: https://www.npmjs.com/signup)

	npm adduser

- publish

	npm publish

6. Usage

	npm install segment-anything-model

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