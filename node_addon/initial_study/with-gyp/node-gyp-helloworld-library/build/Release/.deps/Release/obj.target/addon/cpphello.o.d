cmd_Release/obj.target/addon/cpphello.o := c++ -o Release/obj.target/addon/cpphello.o ../cpphello.cpp '-DNODE_GYP_MODULE_NAME=addon' '-DUSING_UV_SHARED=1' '-DUSING_V8_SHARED=1' '-DV8_DEPRECATION_WARNINGS=1' '-D_GLIBCXX_USE_CXX11_ABI=1' '-D_DARWIN_USE_64_BIT_INODE=1' '-D_LARGEFILE_SOURCE' '-D_FILE_OFFSET_BITS=64' '-DOPENSSL_NO_PINSHARED' '-DOPENSSL_THREADS' '-DBUILDING_NODE_EXTENSION' -I/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node -I/Users/rtous/Library/Caches/node-gyp/20.17.0/src -I/Users/rtous/Library/Caches/node-gyp/20.17.0/deps/openssl/config -I/Users/rtous/Library/Caches/node-gyp/20.17.0/deps/openssl/openssl/include -I/Users/rtous/Library/Caches/node-gyp/20.17.0/deps/uv/include -I/Users/rtous/Library/Caches/node-gyp/20.17.0/deps/zlib -I/Users/rtous/Library/Caches/node-gyp/20.17.0/deps/v8/include  -O3 -gdwarf-2 -mmacosx-version-min=10.15 -arch arm64 -Wall -Wendif-labels -W -Wno-unused-parameter -std=gnu++17 -stdlib=libc++ -fno-rtti -fno-exceptions -fno-strict-aliasing -MMD -MF ./Release/.deps/Release/obj.target/addon/cpphello.o.d.raw   -c
Release/obj.target/addon/cpphello.o: ../cpphello.cpp \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/node.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/cppgc/common.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8config.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-array-buffer.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-local-handle.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-internal.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-version.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-object.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-maybe.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-persistent-handle.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-weak-callback-info.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-primitive.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-data.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-value.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-traced-handle.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-container.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-context.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-snapshot.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-date.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-debug.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-script.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-callbacks.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-promise.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-message.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-exception.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-extension.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-external.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-function.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-function-callback.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-template.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-memory-span.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-initialization.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-isolate.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-embedder-heap.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-microtask.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-statistics.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-unwinder.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-embedder-state-scope.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-platform.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-json.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-locker.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-microtask-queue.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-primitive-object.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-proxy.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-regexp.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-typed-array.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-value-serializer.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-wasm.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/node_version.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/node_api.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/js_native_api.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/js_native_api_types.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/node_api_types.h \
  ../library.h
../cpphello.cpp:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/node.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/cppgc/common.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8config.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-array-buffer.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-local-handle.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-internal.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-version.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-object.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-maybe.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-persistent-handle.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-weak-callback-info.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-primitive.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-data.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-value.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-traced-handle.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-container.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-context.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-snapshot.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-date.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-debug.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-script.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-callbacks.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-promise.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-message.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-exception.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-extension.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-external.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-function.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-function-callback.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-template.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-memory-span.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-initialization.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-isolate.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-embedder-heap.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-microtask.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-statistics.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-unwinder.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-embedder-state-scope.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-platform.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-json.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-locker.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-microtask-queue.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-primitive-object.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-proxy.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-regexp.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-typed-array.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-value-serializer.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/v8-wasm.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/node_version.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/node_api.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/js_native_api.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/js_native_api_types.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/node_api_types.h:
../library.h: