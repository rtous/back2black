cmd_Release/obj.target/lester/binding_utils.o := c++ -o Release/obj.target/lester/binding_utils.o ../binding_utils.cpp '-DNODE_GYP_MODULE_NAME=lester' '-DUSING_UV_SHARED=1' '-DUSING_V8_SHARED=1' '-DV8_DEPRECATION_WARNINGS=1' '-D_GLIBCXX_USE_CXX11_ABI=1' '-D_DARWIN_USE_64_BIT_INODE=1' '-D_LARGEFILE_SOURCE' '-D_FILE_OFFSET_BITS=64' '-DOPENSSL_NO_PINSHARED' '-DOPENSSL_THREADS' '-DBUILDING_NODE_EXTENSION' -I/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node -I/Users/rtous/Library/Caches/node-gyp/20.17.0/src -I/Users/rtous/Library/Caches/node-gyp/20.17.0/deps/openssl/config -I/Users/rtous/Library/Caches/node-gyp/20.17.0/deps/openssl/openssl/include -I/Users/rtous/Library/Caches/node-gyp/20.17.0/deps/uv/include -I/Users/rtous/Library/Caches/node-gyp/20.17.0/deps/zlib -I/Users/rtous/Library/Caches/node-gyp/20.17.0/deps/v8/include -I/Users/rtous/dev/back2black/common -I/Users/rtous/dev/back2black/api -I/Users/rtous/dev/back2black/third-party/sam -I/Users/rtous/dev/opencv/build -I/Users/rtous/dev/opencv/include -I/Users/rtous/dev/opencv/modules/core/include -I/Users/rtous/dev/opencv/modules/calib3d/include -I/Users/rtous/dev/opencv/modules/features2d/include -I/Users/rtous/dev/opencv/modules/flann/include -I/Users/rtous/dev/opencv/modules/dnn/include -I/Users/rtous/dev/opencv/modules/highgui/include -I/Users/rtous/dev/opencv/modules/imgcodecs/include -I/Users/rtous/dev/opencv/modules/video/include -I/Users/rtous/dev/opencv/modules/videoio/include -I/Users/rtous/dev/opencv/modules/imgproc/include -I/Users/rtous/dev/opencv/modules/ml/include -I/Users/rtous/dev/opencv/modules/objdetect/include -I/Users/rtous/dev/opencv/modules/photo/include -I/Users/rtous/dev/opencv/modules/stitching/include  -O3 -gdwarf-2 -mmacosx-version-min=10.15 -arch arm64 -Wall -Wendif-labels -W -Wno-unused-parameter -std=gnu++17 -stdlib=libc++ -fno-strict-aliasing -MMD -MF ./Release/.deps/Release/obj.target/lester/binding_utils.o.d.raw   -c
Release/obj.target/lester/binding_utils.o: ../binding_utils.cpp \
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
  /Users/rtous/dev/opencv/include/opencv2/opencv.hpp \
  /Users/rtous/dev/opencv/build/opencv2/opencv_modules.hpp \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core.hpp \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/cvdef.h \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/version.hpp \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/hal/interface.h \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/cv_cpu_dispatch.h \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/base.hpp \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/cvstd.hpp \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/cvstd_wrapper.hpp \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/neon_utils.hpp \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/vsx_utils.hpp \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/check.hpp \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/traits.hpp \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/matx.hpp \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/saturate.hpp \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/fast_math.hpp \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/types.hpp \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/mat.hpp \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/bufferpool.hpp \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/mat.inl.hpp \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/persistence.hpp \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/operations.hpp \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/cvstd.inl.hpp \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/utility.hpp \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/optim.hpp \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/ovx.hpp \
  /Users/rtous/dev/opencv/modules/calib3d/include/opencv2/calib3d.hpp \
  /Users/rtous/dev/opencv/modules/features2d/include/opencv2/features2d.hpp \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/miniflann.hpp \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/defines.h \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/config.h \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/affine.hpp \
  /Users/rtous/dev/opencv/modules/dnn/include/opencv2/dnn.hpp \
  /Users/rtous/dev/opencv/modules/dnn/include/opencv2/dnn/dnn.hpp \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/async.hpp \
  /Users/rtous/dev/opencv/modules/dnn/include/opencv2/dnn/../dnn/version.hpp \
  /Users/rtous/dev/opencv/modules/dnn/include/opencv2/dnn/dict.hpp \
  /Users/rtous/dev/opencv/modules/dnn/include/opencv2/dnn/layer.hpp \
  /Users/rtous/dev/opencv/modules/dnn/include/opencv2/dnn/dnn.inl.hpp \
  /Users/rtous/dev/opencv/modules/dnn/include/opencv2/dnn/utils/inference_engine.hpp \
  /Users/rtous/dev/opencv/modules/dnn/include/opencv2/dnn/utils/../dnn.hpp \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann.hpp \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/flann_base.hpp \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/general.h \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/matrix.h \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/params.h \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/any.h \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/saving.h \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/nn_index.h \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/result_set.h \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/all_indices.h \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/kdtree_index.h \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/dynamic_bitset.h \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/dist.h \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/heap.h \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/allocator.h \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/random.h \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/kdtree_single_index.h \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/kmeans_index.h \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/logger.h \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/composite_index.h \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/linear_index.h \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/hierarchical_clustering_index.h \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/lsh_index.h \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/lsh_table.h \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/autotuned_index.h \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/ground_truth.h \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/index_testing.h \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/timer.h \
  /Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/sampling.h \
  /Users/rtous/dev/opencv/modules/highgui/include/opencv2/highgui.hpp \
  /Users/rtous/dev/opencv/modules/imgcodecs/include/opencv2/imgcodecs.hpp \
  /Users/rtous/dev/opencv/modules/videoio/include/opencv2/videoio.hpp \
  /Users/rtous/dev/opencv/modules/imgproc/include/opencv2/imgproc.hpp \
  /Users/rtous/dev/opencv/modules/imgproc/include/opencv2/./imgproc/segmentation.hpp \
  /Users/rtous/dev/opencv/modules/ml/include/opencv2/ml.hpp \
  /Users/rtous/dev/opencv/modules/ml/include/opencv2/ml/ml.inl.hpp \
  /Users/rtous/dev/opencv/modules/objdetect/include/opencv2/objdetect.hpp \
  /Users/rtous/dev/opencv/modules/objdetect/include/opencv2/objdetect/aruco_detector.hpp \
  /Users/rtous/dev/opencv/modules/objdetect/include/opencv2/objdetect/aruco_dictionary.hpp \
  /Users/rtous/dev/opencv/modules/objdetect/include/opencv2/objdetect/aruco_board.hpp \
  /Users/rtous/dev/opencv/modules/objdetect/include/opencv2/objdetect/graphical_code_detector.hpp \
  /Users/rtous/dev/opencv/modules/objdetect/include/opencv2/objdetect/detection_based_tracker.hpp \
  /Users/rtous/dev/opencv/modules/objdetect/include/opencv2/objdetect/face.hpp \
  /Users/rtous/dev/opencv/modules/objdetect/include/opencv2/objdetect/charuco_detector.hpp \
  /Users/rtous/dev/opencv/modules/objdetect/include/opencv2/objdetect/barcode.hpp \
  /Users/rtous/dev/opencv/modules/photo/include/opencv2/photo.hpp \
  /Users/rtous/dev/opencv/modules/stitching/include/opencv2/stitching.hpp \
  /Users/rtous/dev/opencv/modules/stitching/include/opencv2/stitching/warpers.hpp \
  /Users/rtous/dev/opencv/modules/stitching/include/opencv2/stitching/detail/warpers.hpp \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/cuda.hpp \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/cuda_types.hpp \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/cuda.inl.hpp \
  /Users/rtous/dev/opencv/modules/stitching/include/opencv2/stitching/detail/warpers_inl.hpp \
  /Users/rtous/dev/opencv/modules/stitching/include/opencv2/stitching/detail/matchers.hpp \
  /Users/rtous/dev/opencv/modules/stitching/include/opencv2/stitching/detail/motion_estimators.hpp \
  /Users/rtous/dev/opencv/modules/stitching/include/opencv2/stitching/detail/util.hpp \
  /Users/rtous/dev/opencv/modules/stitching/include/opencv2/stitching/detail/util_inl.hpp \
  /Users/rtous/dev/opencv/modules/stitching/include/opencv2/stitching/detail/camera.hpp \
  /Users/rtous/dev/opencv/modules/stitching/include/opencv2/stitching/detail/exposure_compensate.hpp \
  /Users/rtous/dev/opencv/modules/stitching/include/opencv2/stitching/detail/seam_finders.hpp \
  /Users/rtous/dev/opencv/modules/stitching/include/opencv2/stitching/detail/blenders.hpp \
  /Users/rtous/dev/opencv/modules/video/include/opencv2/video.hpp \
  /Users/rtous/dev/opencv/modules/video/include/opencv2/video/tracking.hpp \
  /Users/rtous/dev/opencv/modules/video/include/opencv2/video/background_segm.hpp \
  /Users/rtous/dev/back2black/api/api.h \
  /Users/rtous/dev/opencv/modules/core/include/opencv2/core/utils/filesystem.hpp \
  /Users/rtous/dev/back2black/third-party/sam/sam.h ../wrappedobject.h \
  /Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/node_object_wrap.h \
  ../binding_utils.h
../binding_utils.cpp:
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
/Users/rtous/dev/opencv/include/opencv2/opencv.hpp:
/Users/rtous/dev/opencv/build/opencv2/opencv_modules.hpp:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core.hpp:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/cvdef.h:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/version.hpp:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/hal/interface.h:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/cv_cpu_dispatch.h:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/base.hpp:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/cvstd.hpp:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/cvstd_wrapper.hpp:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/neon_utils.hpp:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/vsx_utils.hpp:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/check.hpp:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/traits.hpp:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/matx.hpp:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/saturate.hpp:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/fast_math.hpp:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/types.hpp:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/mat.hpp:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/bufferpool.hpp:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/mat.inl.hpp:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/persistence.hpp:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/operations.hpp:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/cvstd.inl.hpp:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/utility.hpp:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/optim.hpp:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/ovx.hpp:
/Users/rtous/dev/opencv/modules/calib3d/include/opencv2/calib3d.hpp:
/Users/rtous/dev/opencv/modules/features2d/include/opencv2/features2d.hpp:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/miniflann.hpp:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/defines.h:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/config.h:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/affine.hpp:
/Users/rtous/dev/opencv/modules/dnn/include/opencv2/dnn.hpp:
/Users/rtous/dev/opencv/modules/dnn/include/opencv2/dnn/dnn.hpp:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/async.hpp:
/Users/rtous/dev/opencv/modules/dnn/include/opencv2/dnn/../dnn/version.hpp:
/Users/rtous/dev/opencv/modules/dnn/include/opencv2/dnn/dict.hpp:
/Users/rtous/dev/opencv/modules/dnn/include/opencv2/dnn/layer.hpp:
/Users/rtous/dev/opencv/modules/dnn/include/opencv2/dnn/dnn.inl.hpp:
/Users/rtous/dev/opencv/modules/dnn/include/opencv2/dnn/utils/inference_engine.hpp:
/Users/rtous/dev/opencv/modules/dnn/include/opencv2/dnn/utils/../dnn.hpp:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann.hpp:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/flann_base.hpp:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/general.h:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/matrix.h:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/params.h:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/any.h:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/saving.h:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/nn_index.h:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/result_set.h:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/all_indices.h:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/kdtree_index.h:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/dynamic_bitset.h:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/dist.h:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/heap.h:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/allocator.h:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/random.h:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/kdtree_single_index.h:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/kmeans_index.h:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/logger.h:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/composite_index.h:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/linear_index.h:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/hierarchical_clustering_index.h:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/lsh_index.h:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/lsh_table.h:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/autotuned_index.h:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/ground_truth.h:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/index_testing.h:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/timer.h:
/Users/rtous/dev/opencv/modules/flann/include/opencv2/flann/sampling.h:
/Users/rtous/dev/opencv/modules/highgui/include/opencv2/highgui.hpp:
/Users/rtous/dev/opencv/modules/imgcodecs/include/opencv2/imgcodecs.hpp:
/Users/rtous/dev/opencv/modules/videoio/include/opencv2/videoio.hpp:
/Users/rtous/dev/opencv/modules/imgproc/include/opencv2/imgproc.hpp:
/Users/rtous/dev/opencv/modules/imgproc/include/opencv2/./imgproc/segmentation.hpp:
/Users/rtous/dev/opencv/modules/ml/include/opencv2/ml.hpp:
/Users/rtous/dev/opencv/modules/ml/include/opencv2/ml/ml.inl.hpp:
/Users/rtous/dev/opencv/modules/objdetect/include/opencv2/objdetect.hpp:
/Users/rtous/dev/opencv/modules/objdetect/include/opencv2/objdetect/aruco_detector.hpp:
/Users/rtous/dev/opencv/modules/objdetect/include/opencv2/objdetect/aruco_dictionary.hpp:
/Users/rtous/dev/opencv/modules/objdetect/include/opencv2/objdetect/aruco_board.hpp:
/Users/rtous/dev/opencv/modules/objdetect/include/opencv2/objdetect/graphical_code_detector.hpp:
/Users/rtous/dev/opencv/modules/objdetect/include/opencv2/objdetect/detection_based_tracker.hpp:
/Users/rtous/dev/opencv/modules/objdetect/include/opencv2/objdetect/face.hpp:
/Users/rtous/dev/opencv/modules/objdetect/include/opencv2/objdetect/charuco_detector.hpp:
/Users/rtous/dev/opencv/modules/objdetect/include/opencv2/objdetect/barcode.hpp:
/Users/rtous/dev/opencv/modules/photo/include/opencv2/photo.hpp:
/Users/rtous/dev/opencv/modules/stitching/include/opencv2/stitching.hpp:
/Users/rtous/dev/opencv/modules/stitching/include/opencv2/stitching/warpers.hpp:
/Users/rtous/dev/opencv/modules/stitching/include/opencv2/stitching/detail/warpers.hpp:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/cuda.hpp:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/cuda_types.hpp:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/cuda.inl.hpp:
/Users/rtous/dev/opencv/modules/stitching/include/opencv2/stitching/detail/warpers_inl.hpp:
/Users/rtous/dev/opencv/modules/stitching/include/opencv2/stitching/detail/matchers.hpp:
/Users/rtous/dev/opencv/modules/stitching/include/opencv2/stitching/detail/motion_estimators.hpp:
/Users/rtous/dev/opencv/modules/stitching/include/opencv2/stitching/detail/util.hpp:
/Users/rtous/dev/opencv/modules/stitching/include/opencv2/stitching/detail/util_inl.hpp:
/Users/rtous/dev/opencv/modules/stitching/include/opencv2/stitching/detail/camera.hpp:
/Users/rtous/dev/opencv/modules/stitching/include/opencv2/stitching/detail/exposure_compensate.hpp:
/Users/rtous/dev/opencv/modules/stitching/include/opencv2/stitching/detail/seam_finders.hpp:
/Users/rtous/dev/opencv/modules/stitching/include/opencv2/stitching/detail/blenders.hpp:
/Users/rtous/dev/opencv/modules/video/include/opencv2/video.hpp:
/Users/rtous/dev/opencv/modules/video/include/opencv2/video/tracking.hpp:
/Users/rtous/dev/opencv/modules/video/include/opencv2/video/background_segm.hpp:
/Users/rtous/dev/back2black/api/api.h:
/Users/rtous/dev/opencv/modules/core/include/opencv2/core/utils/filesystem.hpp:
/Users/rtous/dev/back2black/third-party/sam/sam.h:
../wrappedobject.h:
/Users/rtous/Library/Caches/node-gyp/20.17.0/include/node/node_object_wrap.h:
../binding_utils.h:
