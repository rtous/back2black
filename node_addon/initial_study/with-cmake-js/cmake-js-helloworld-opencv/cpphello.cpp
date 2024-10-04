#include <napi.h>
#include <opencv2/opencv.hpp>

using namespace cv;

Napi::String Method(const Napi::CallbackInfo& info) {
  Mat output = Mat::zeros( 120, 350, CV_8UC3 );
  Napi::Env env = info.Env();
  return Napi::String::New(env, "world");
}

Napi::Object Initialize(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "hello"),
              Napi::Function::New(env, Method));
  return exports;
}

NODE_API_MODULE(addon, Initialize)