IDEAS:

- Turn C++ code into a Node.js addon- 
- Two ways: gyp or cmake-js
- gyp is simpler but cmake-js let you use CMakeLists.txt files.
- I've been able to build with opencv in both ways.


LINKS:

Official tutorial: 

https://nodejs.org/api/addons.html
https://github.com/nodejs/node-gyp

Other tutorials:

https://medium.com/@koistya/how-to-call-c-c-code-from-node-js-86a773033892
https://github.com/greuze/node-gyp-examples/blob/master/hello-world/test.js

Tutorial with libraries:

https://blog.s1h.org/shipping-node-addons-with-libraries/
https://www.anthonyvardaro.com/blog/how-to-link-dll-gyp

Tutorial for OpenCV:

https://stackoverflow.com/questions/50196514/most-basic-example-of-adding-a-opencv-c-add-on-to-node-js


POTENTIAL PROBLEM:

Potential incompatibility with Migw64

https://stackoverflow.com/questions/54632751/how-to-use-a-static-library-with-node-gyp-on-windows