// myobject.cc
#include "wrappedobject.h"
#include "binding_utils.h"
#include "filesystem_header.h"


namespace demo {

using v8::Context; 
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::ObjectTemplate; 
using v8::String;
using v8::Value;

/*MyObject::MyObject(double value) : value_(value) {  
  anAPIState.set_checkpoint_path("checkpoints/ggml-model-f16.bin");
  anAPIState.load_model();
}
*/
MyObject::MyObject(std::string addon_path) : addon_path_(addon_path) {  
  std::string checkpoint_path = fs::path(addon_path).parent_path().u8string();
  //std::string checkpoint_path = ".";
  anAPIState.set_checkpoint_path(checkpoint_path+"/checkpoints/ggml-model-f16.bin");
  anAPIState.load_model();
}

MyObject::~MyObject() {
}

void MyObject::Init(Local<Object> exports) {
  Isolate* isolate = exports->GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();

  Local<ObjectTemplate> addon_data_tpl = ObjectTemplate::New(isolate);
  addon_data_tpl->SetInternalFieldCount(1);  // 1 field for the MyObject::New()
  Local<Object> addon_data =
      addon_data_tpl->NewInstance(context).ToLocalChecked();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New, addon_data);
  tpl->SetClassName(String::NewFromUtf8(isolate, "MyObject").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  //NODE_SET_PROTOTYPE_METHOD(tpl, "plusOne", PlusOne); //Not used, just to keep the how to.
  NODE_SET_PROTOTYPE_METHOD(tpl, "precompute_image", precompute_image);
  NODE_SET_PROTOTYPE_METHOD(tpl, "compute_mask", compute_mask);

  Local<Function> constructor = tpl->GetFunction(context).ToLocalChecked();
  addon_data->SetInternalField(0, constructor);
  exports->Set(context, String::NewFromUtf8(
      isolate, "MyObject").ToLocalChecked(),
      constructor).FromJust();
}

void MyObject::New(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext(); 

  if (args.IsConstructCall()) {
    // Invoked as constructor: `new MyObject(...)`
    /*double value = args[0]->IsUndefined() ?
        0 : args[0]->NumberValue(context).FromMaybe(0);
    MyObject* obj = new MyObject(value);*/
    /*std::string node_path = args[0]->IsUndefined() ?
        "./build/Release" : args[0]->Utf8Value(context).FromMaybe(0);*/
    //std::string node_path = String::Utf8Value 

    /*Local<Object> arg1 =  args[0]->ToObject(context).ToLocalChecked();
    Local<v8::String> aString = args[0].As<v8::String>();
    std::string node_path = aString>Buffer();*/

    v8::String::Utf8Value str(isolate, args[0]);
    std::string node_path(*str);

    MyObject* obj = new MyObject(node_path);
    obj->Wrap(args.This()); 
    args.GetReturnValue().Set(args.This());
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    Local<Function> cons =
        args.Data().As<Object>()->GetInternalField(0)
            .As<Value>().As<Function>();
    Local<Object> result =
        cons->NewInstance(context, argc, argv).ToLocalChecked();
    args.GetReturnValue().Set(result);
  }
}

//An example method 
//Not used, just to keep the how to.
/*
void MyObject::PlusOne(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  MyObject* obj = ObjectWrap::Unwrap<MyObject>(args.This());
  obj->value_ += 1;

  args.GetReturnValue().Set(Number::New(isolate, obj->value_));
}
*/

//precompute_image
void MyObject::precompute_image(const FunctionCallbackInfo<Value>& args) {
  printf("MyObject::precompute_image...\n");
  Isolate* isolate = args.GetIsolate();
  MyObject* obj = ObjectWrap::Unwrap<MyObject>(args.This());
  cv::Mat opencv_img; 
  imageArg2OpenCV(args, opencv_img);  
  obj->anAPIState.precompute_image_opencv(opencv_img);
  //args.GetReturnValue().Set(Number::New(isolate, obj->value_));
}

//precompute_image
void MyObject::compute_mask(const FunctionCallbackInfo<Value>& args) {
  printf("MyObject::compute_mask...\n");
  Isolate* isolate = args.GetIsolate();
  MyObject* obj = ObjectWrap::Unwrap<MyObject>(args.This());
  cv::Mat output_img;
  int x = 539;
  int y = 309;
  bool found = obj->anAPIState.compute_mask_opencv(x, y, output_img);
  if (found) {
    printf("Mask found.\n"); 
    returnOpenCVImage(args, output_img);
  } else
    printf("No mask found :-(\n");
   
}

}  // namespace demo