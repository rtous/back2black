#include <node.h>
#include <opencv2/opencv.hpp>
#include "api.h"
#include "library.h"

using namespace cv;

namespace demo {

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;
using v8::Context;
using v8::ArrayBuffer;

/*
Methods for receiving and passing images have been done with the help of these links:

  https://github.com/lgybetter/opencv-node-addon
  https://nodejs.org/api/addons.html
  https://blog.risingstack.com/using-buffers-node-js-c-plus-plus/
  https://www.dynamsoft.com/codepool/nodejs-document-detection-rectification.html
  https://stackoverflow.com/questions/39796251/copy-data-into-v8arraybuffer

Alternative methods based in the nan library have been discarded:

  https://blog.risingstack.com/using-buffers-node-js-c-plus-plus/
*/

// Hello World method
void hello(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Mat output = Mat::zeros( 120, 350, CV_8UC3 );
  //Mat img = test();
  myLibrary();
  Mat img = test();
  test2();
  args.GetReturnValue().Set(String::NewFromUtf8(
      isolate, "world").ToLocalChecked());
}

//Extracts an image from args[0] and converts it to opencv
//The input data comes directly from the data of a javascript fs.readFile
void imageArg2OpenCV(const FunctionCallbackInfo<Value>& args, cv::Mat &opencv_img) {
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();
  Local<Object> arg1 =  args[0]->ToObject(context).ToLocalChecked();
  Local<v8::Uint8Array> array = args[0].As<v8::Uint8Array>();
  Local<ArrayBuffer> ab = array->Buffer();
  long size1 = ab->ByteLength();
  uchar* buffer1 = static_cast<uchar*>(ab->Data());
  std::vector<uchar> vec1(buffer1, buffer1 + (unsigned int) size1);
  opencv_img = cv::imdecode(vec1, CV_8U);
}

// Receives an image from javascript
void image_javascript2C(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  cv::Mat opencv_img;
  imageArg2OpenCV(args, opencv_img);
  int img_width = opencv_img.cols;
  cv::imwrite("output.jpg", opencv_img);
  args.GetReturnValue().Set(v8::Integer::New(isolate, img_width));
}

//Sends an image to javascript
void image_C2javascript(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();
  cv::Mat img_decode1;
  img_decode1 = cv::imread("img.jpg", cv::IMREAD_COLOR);
  static std::vector<uchar> buf_vector;
  cv::imencode(".jpg", img_decode1, buf_vector);
  v8::Handle<v8::ArrayBuffer> abh = v8::ArrayBuffer::New(isolate, buf_vector.size());
  std::memcpy(abh->Data(), buf_vector.data(), buf_vector.size());
  args.GetReturnValue().Set(abh);
}

void Initialize(Local<Object> exports) {
  NODE_SET_METHOD(exports, "hello", hello);
  NODE_SET_METHOD(exports, "image_javascript2C", image_javascript2C);
  NODE_SET_METHOD(exports, "image_C2javascript", image_C2javascript);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)

}  // namespace demo 