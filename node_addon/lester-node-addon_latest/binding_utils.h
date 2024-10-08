#ifndef __BINDINGUTILS_H__
#define __BINDINGUTILS_H__

namespace demo {

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;
using v8::Context;
using v8::ArrayBuffer;

void imageArg2OpenCV(const FunctionCallbackInfo<Value>& args, cv::Mat &opencv_img);
void returnOpenCVImage(const FunctionCallbackInfo<Value>& args, cv::Mat img);

#endif

}  // namespace demo 