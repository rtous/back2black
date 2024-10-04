{
  "targets": [
    {
      "target_name": "lester",
      "sources": [ 
          "cpphello.cpp"
      ],
      "libraries": [
      "/Users/rtous/dev/back2black/node_addon/with-gyp/node-gyp-helloworld-library/library.a",
      "/Users/rtous/dev/back2black/build_macos/api/liblester-api.a",
      "Accelerate.framework",
      "OpenCL.framework",
      "/Users/rtous/dev/opencv/build/lib/libopencv_core.a",
      "/Users/rtous/dev/opencv/build/lib/libopencv_imgcodecs.a",
      "/Users/rtous/dev/opencv/build/lib/libopencv_imgproc.a",
      "/Users/rtous/dev/opencv/build/lib/libopencv_features2d.a",
      "/Users/rtous/dev/opencv/build/lib/libopencv_gapi.a",
      "/Users/rtous/dev/opencv/build/lib/libopencv_highgui.a",
      "/Users/rtous/dev/opencv/build/lib/libopencv_objdetect.a",
      "/Users/rtous/dev/opencv/build/lib/libopencv_photo.a",
      "/Users/rtous/dev/opencv/build/lib/libopencv_stitching.a",
      "/Users/rtous/dev/opencv/build/lib/libopencv_ts.a",
      "/Users/rtous/dev/opencv/build/lib/libopencv_video.a",
      "/Users/rtous/dev/opencv/build/lib/libopencv_videoio.a",
      "/Users/rtous/dev/opencv/build/3rdparty/lib/libtegra_hal.a",
      "/Users/rtous/dev/opencv/build/3rdparty/lib/libzlib.a",
      "/Users/rtous/dev/opencv/build/3rdparty/lib/liblibwebp.a",
      "/Users/rtous/dev/opencv/build/3rdparty/lib/libade.a",
      "/Users/rtous/dev/opencv/build/3rdparty/lib/libIlmImf.a",
      "/Users/rtous/dev/opencv/build/3rdparty/lib/libittnotify.a",
      "/Users/rtous/dev/opencv/build/3rdparty/lib/liblibjpeg-turbo.a",
      "/Users/rtous/dev/opencv/build/3rdparty/lib/liblibopenjp2.a",
      "/Users/rtous/dev/opencv/build/3rdparty/lib/liblibprotobuf.a",
      "/Users/rtous/dev/opencv/build/3rdparty/lib/liblibtiff.a",
      "/Users/rtous/dev/opencv/build/3rdparty/lib/liblibpng.a",
      ],
      "include_dirs": [
        "/Users/rtous/dev/back2black/node_addon/with-gyp/node-gyp-helloworld-library",
        "/Users/rtous/dev/back2black/api",
        "/Users/rtous/dev/opencv/build",
        "/Users/rtous/dev/opencv/include",
        "/Users/rtous/dev/opencv/modules/core/include",
        "/Users/rtous/dev/opencv/modules/calib3d/include",
        "/Users/rtous/dev/opencv/modules/features2d/include",
        "/Users/rtous/dev/opencv/modules/flann/include",
        "/Users/rtous/dev/opencv/modules/dnn/include",
        "/Users/rtous/dev/opencv/modules/highgui/include",
        "/Users/rtous/dev/opencv/modules/imgcodecs/include",
        "/Users/rtous/dev/opencv/modules/video/include",
        "/Users/rtous/dev/opencv/modules/videoio/include",
        "/Users/rtous/dev/opencv/modules/imgproc/include",
        "/Users/rtous/dev/opencv/modules/ml/include",
        "/Users/rtous/dev/opencv/modules/objdetect/include",
        "/Users/rtous/dev/opencv/modules/photo/include",
        "/Users/rtous/dev/opencv/modules/stitching/include",
        "/Users/rtous/dev/opencv/modules/ml/include",
        "/Users/rtous/dev/opencv/modules/ml/include",
        "/Users/rtous/dev/opencv/modules/ml/include",
        "/Users/rtous/dev/opencv/modules/ml/include",
        #"<(module_root_dir)/tobii/include"
      ],
      "cflags_cc!": [
          "-fno-rtti",
          "-fno-exceptions"
      ],
      "xcode_settings": {
          "GCC_ENABLE_CPP_RTTI": "YES",
          "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
      },
      'defines': [
          #'WITH_ITT=OFF', #-D WITH_ITT=OFF \
        ],
    }
  ],
} 