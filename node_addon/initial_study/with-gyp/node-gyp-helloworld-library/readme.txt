gcc -c library.cpp -o library.o
ar rcs library.a library.o
//gcc library_test.cpp library.o -o library_test

node-gyp configure build
node test.js