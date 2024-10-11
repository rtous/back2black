{
  "targets": [
    {
      "target_name": "lester",
      "output_dir3": "build2",
      "sources": [ 
          "binding_utils.cpp",
          "cpphello.cpp",
          "wrappedobject.cpp"
      ],
      'conditions': [
        [ 'OS=="mac"', {
          "libraries": [
          "<(module_root_dir)/../../build_macos/api/liblester-api.a",
          "<(module_root_dir)/../../build_macos/common/libcommon.a",
          "<(module_root_dir)/../../build_macos/third-party/sam/libsam.a",
          "<(module_root_dir)/../../build_macos/third-party/ggml/src/libggml.a",
          #"Accelerate.framework",
          #"OpenCL.framework",
          "<(module_root_dir)/../../../opencv/build/lib/libopencv_core.a",
          "<(module_root_dir)/../../../opencv/build/lib/libopencv_imgcodecs.a",
          "<(module_root_dir)/../../../opencv/build/lib/libopencv_imgproc.a",
          "<(module_root_dir)/../../../opencv/build/lib/libopencv_features2d.a",
          "<(module_root_dir)/../../../opencv/build/lib/libopencv_gapi.a",
          "<(module_root_dir)/../../../opencv/build/lib/libopencv_highgui.a",
          "<(module_root_dir)/../../../opencv/build/lib/libopencv_objdetect.a",
          "<(module_root_dir)/../../../opencv/build/lib/libopencv_photo.a",
          "<(module_root_dir)/../../../opencv/build/lib/libopencv_stitching.a",
          "<(module_root_dir)/../../../opencv/build/lib/libopencv_ts.a",
          "<(module_root_dir)/../../../opencv/build/lib/libopencv_video.a",
          "<(module_root_dir)/../../../opencv/build/lib/libopencv_videoio.a",
          "<(module_root_dir)/../../../opencv/build/3rdparty/lib/libtegra_hal.a",
          "<(module_root_dir)/../../../opencv/build/3rdparty/lib/libzlib.a",
          "<(module_root_dir)/../../../opencv/build/3rdparty/lib/liblibwebp.a",
          "<(module_root_dir)/../../../opencv/build/3rdparty/lib/libade.a",
          "<(module_root_dir)/../../../opencv/build/3rdparty/lib/libIlmImf.a",
          "<(module_root_dir)/../../../opencv/build/3rdparty/lib/libittnotify.a",
          "<(module_root_dir)/../../../opencv/build/3rdparty/lib/liblibjpeg-turbo.a",
          "<(module_root_dir)/../../../opencv/build/3rdparty/lib/liblibopenjp2.a",
          "<(module_root_dir)/../../../opencv/build/3rdparty/lib/liblibprotobuf.a",
          "<(module_root_dir)/../../../opencv/build/3rdparty/lib/liblibtiff.a",
          "<(module_root_dir)/../../../opencv/build/3rdparty/lib/liblibpng.a",
          ],
        }
        ],
        ["OS==\"win\" and target_arch==\"x64\"", {
          #this does not work???
        }
        ],
      ],
	  #These are for Windows (the condition does not work i don't know why)
	  'msvs_settings': {
		'VCCLCompilerTool': {
			'AdditionalOptions': [
				#'/GR',
				'/MTd',
				#'/EHsc'
			]
		},
	  },
	  "libraries": [
	  "<(module_root_dir)/../../build_windows/api/Debug/lester-api.lib",
	  "<(module_root_dir)/../../build_windows/common/Debug/common.lib",
	  "<(module_root_dir)/../../build_windows/third-party/sam/Debug/sam.lib",
	  "<(module_root_dir)/../../build_windows/third-party/ggml/src/Debug/ggml.lib",
	  "<(module_root_dir)/../../../opencv/build/lib/Debug/opencv_core490d.lib",
	  "<(module_root_dir)/../../../opencv/build/lib/Debug/opencv_imgcodecs490d.lib",
	  "<(module_root_dir)/../../../opencv/build/lib/Debug/opencv_imgproc490d.lib",
	  "<(module_root_dir)/../../../opencv/build/lib/Debug/opencv_features2d490d.lib",
	  "<(module_root_dir)/../../../opencv/build/lib/Debug/opencv_gapi490d.lib",
	  "<(module_root_dir)/../../../opencv/build/lib/Debug/opencv_highgui490d.lib",
	  "<(module_root_dir)/../../../opencv/build/lib/Debug/opencv_objdetect490d.lib",
	  "<(module_root_dir)/../../../opencv/build/lib/Debug/opencv_photo490d.lib",
	  "<(module_root_dir)/../../../opencv/build/lib/Debug/opencv_stitching490d.lib",
	  "<(module_root_dir)/../../../opencv/build/lib/Debug/opencv_ts490d.lib",
	  "<(module_root_dir)/../../../opencv/build/lib/Debug/opencv_video490d.lib",
	  "<(module_root_dir)/../../../opencv/build/lib/Debug/opencv_videoio490d.lib",
	  "<(module_root_dir)/../../../opencv/build/3rdparty/lib/Debug/zlibd.lib",
	  "<(module_root_dir)/../../../opencv/build/3rdparty/lib/Debug/libwebpd.lib",
	  "<(module_root_dir)/../../../opencv/build/3rdparty/lib/Debug/aded.lib",
	  "<(module_root_dir)/../../../opencv/build/3rdparty/lib/Debug/IlmImfd.lib",
	  "<(module_root_dir)/../../../opencv/build/3rdparty/lib/Debug/ittnotifyd.lib",
	  "<(module_root_dir)/../../../opencv/build/3rdparty/lib/Debug/libjpeg-turbod.lib",
	  "<(module_root_dir)/../../../opencv/build/3rdparty/lib/Debug/libopenjp2d.lib",
	  "<(module_root_dir)/../../../opencv/build/3rdparty/lib/Debug/libprotobufd.lib",
	  "<(module_root_dir)/../../../opencv/build/3rdparty/lib/Debug/libtiffd.lib",
	  "<(module_root_dir)/../../../opencv/build/3rdparty/lib/Debug/libpngd.lib",
	  "<(module_root_dir)/../../../opencv/build/3rdparty/lib/Debug/ippiwd.lib",
	  "<(module_root_dir)/../../../opencv/build/3rdparty/ippicv/ippicv_win/icv/lib/intel64/ippicvmt.lib",
	  ],
	  ##############
      "include_dirs": [
        "<(module_root_dir)/../../common",
        "<(module_root_dir)/../../api",
        "<(module_root_dir)/../../third-party/sam",
        #"/Users/rtous/dev/back2black/third-party/ggml/include/ggml",
        "<(module_root_dir)/../../../opencv/build",
        "<(module_root_dir)/../../../opencv/include",
        "<(module_root_dir)/../../../opencv/modules/core/include",
        "<(module_root_dir)/../../../opencv/modules/calib3d/include",
        "<(module_root_dir)/../../../opencv/modules/features2d/include",
        "<(module_root_dir)/../../../opencv/modules/flann/include",
        "<(module_root_dir)/../../../opencv/modules/dnn/include",
        "<(module_root_dir)/../../../opencv/modules/highgui/include",
        "<(module_root_dir)/../../../opencv/modules/imgcodecs/include",
        "<(module_root_dir)/../../../opencv/modules/video/include",
        "<(module_root_dir)/../../../opencv/modules/videoio/include",
        "<(module_root_dir)/../../../opencv/modules/imgproc/include",
        "<(module_root_dir)/../../../opencv/modules/ml/include",
        "<(module_root_dir)/../../../opencv/modules/objdetect/include",
        "<(module_root_dir)/../../../opencv/modules/photo/include",
        "<(module_root_dir)/../../../opencv/modules/stitching/include",
      ],
      #"cflags_cc!": [
      #    "-fno-rtti",
      #    "-fno-exceptions"
      #],
      "xcode_settings": {
          "GCC_ENABLE_CPP_RTTI": "YES",
          "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
      },
      'defines': [
          #'WITH_ITT=OFF', #-D WITH_ITT=OFF \
        ],
      'conditions': [
        [ 'OS=="mac"', {
          "libraries": [          
            "Accelerate.framework",
            "OpenCL.framework",
          ]
          #'libraries': [ '-framework Carbon' ],
          #'defines!': [
          #  'PLATFORM="mac"',
          #],
          #'defines': [
            # we need to use node's preferred "darwin" rather than gyp's preferred "mac"
          #  'PLATFORM="darwin"',
          #],
        }
        ],
        [ 'OS=="win"', {
          #'sources': [
          #  'src/res/node.rc',
          #],
          #'defines': [
          #  'FD_SETSIZE=1024',
            # we need to use node's preferred "win32" rather than gyp's preferred "win"
          #  'PLATFORM="win32"',
          #  '_UNICODE=1',
          #],
          #'libraries': [ '-lpsapi.lib' ]
        }, 
        { # POSIX
          #'defines': [ '__POSIX__' ],
        }
        ],
      ],#conditions
      #"copies":[
      #  { 
      #      'destination': '<(module_root_dir)/build/Release/checkpoints',
      #      'files':[
      #          '<(module_root_dir)/../../checkpoints/ggml-model-f16.bin',
      #      ]
      #  },
      #]
    },#first target
    {#second target
      "target_name": "lester_deploy",
      "type": "none",
      "dependencies": [
        "lester"
      ],
      "conditions": [
        ["OS==\"win\" and target_arch==\"x64\"", {
          "copies": [{
            "destination": "<(module_root_dir)/publish/segment-anything-model-win64/bin/",
            "files": [
              "<(module_root_dir)/build/Release/lester.node"
            ]
          },
          {
            "destination": "<(module_root_dir)/publish/segment-anything-model-win64/bin/checkpoints",
            "files": [
              "<(module_root_dir)/../../checkpoints/ggml-model-f16.bin"
            ]
          },],
        }],
        ["OS==\"win\" and target_arch!=\"x64\"", {
          "copies": [{
            "destination": "<(module_root_dir)/publish/segment-anything-model-linux_win32/bin/",
            "files": [
              "<(module_root_dir)/build/Release/lester.node"
            ]
          },
          {
            "destination": "<(module_root_dir)/publish/segment-anything-model-linux_win32/bin/checkpoints",
            "files": [
              "<(module_root_dir)/../../checkpoints/ggml-model-f16.bin"
            ]
          },]
        }],
        ["OS==\"linux\" and target_arch==\"x64\"", {
          "copies": [{
            "destination": "<(module_root_dir)/publish/segment-anything-model-linux_amd64/bin/",
            "files": [
              "<(module_root_dir)/build/Release/lester.node"
            ]
          },
          {
            "destination": "<(module_root_dir)/publish/segment-anything-model-linux_amd64/bin/checkpoints",
            "files": [
              "<(module_root_dir)/../../checkpoints/ggml-model-f16.bin"
            ]
          },]
        }],
        ["OS==\"linux\" and target_arch!=\"x64\"", {
          "copies": [{
            "destination": "<(module_root_dir)/publish/segment-anything-model-linux_x86/bin/",
            "files": [
              "<(module_root_dir)/build/Release/lester.node"
            ]
          },
          {
            "destination": "<(module_root_dir)/publish/segment-anything-model-linux_x86/bin/checkpoints",
            "files": [
              "<(module_root_dir)/../../checkpoints/ggml-model-f16.bin"
            ]
          },]
        }],
        ["OS==\"mac\"", {
          "copies": [{
            "destination": "<(module_root_dir)/publish/segment-anything-model-mac/bin/",
            "files": [
              "<(module_root_dir)/build/Release/lester.node"
            ]
          },
          {
            "destination": "<(module_root_dir)/publish/segment-anything-model-mac/bin/checkpoints",
            "files": [
              "<(module_root_dir)/../../checkpoints/ggml-model-f16.bin"
            ]
          },
          ]
        }]
      ]
    }#second target
  ],#targets
} 