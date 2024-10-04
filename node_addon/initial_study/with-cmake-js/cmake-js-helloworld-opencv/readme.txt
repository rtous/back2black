SETUP

npm install cmake-js
npm install -g cmake-js
npm install --save-dev node-addon-api


BUILD

rm -rf build
cmake-js build 

TEST

node test.js

