// myobject.h
#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <node.h>
#include <node_object_wrap.h>
#include "api.h"

namespace demo {

class MyObject : public node::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);

 private:
    //explicit MyObject(double value = 0);
    explicit MyObject(std::string addon_path);
    ~MyObject();

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  //static void PlusOne(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void precompute_image(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void compute_mask(const v8::FunctionCallbackInfo<v8::Value>& args);

  //field
  std::string addon_path_;
  //double value_; //Not used, just to keep the how to.
  APIState anAPIState; //INFO: this already calls APIState(); 
};

}  // namespace demo

#endif 