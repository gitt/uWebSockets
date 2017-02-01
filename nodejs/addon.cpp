#include "../src/uWS.h"
#include "addon.h"

void resEnd(const FunctionCallbackInfo<Value> &args) {
    uWS::HttpResponse *res = (uWS::HttpResponse *) args.Holder()->GetAlignedPointerFromInternalField(0);

    if (!res) {
        std::cout << "This response has closed!" << std::endl;
        return;
    }

    NativeString nativeString(args[0]);

    ((Persistent<Value> *) &res->userData)->Reset();
    ((Persistent<Value> *) &res->userData)->~Persistent<Value>();
    res->end(nativeString.getData(), nativeString.getLength());
}

void Main(Local<Object> exports) {
    Isolate *isolate = exports->GetIsolate();

    exports->Set(String::NewFromUtf8(isolate, "server"), Namespace<uWS::SERVER>(isolate).object);
    exports->Set(String::NewFromUtf8(isolate, "client"), Namespace<uWS::CLIENT>(isolate).object);

    // template for response
    Local<ObjectTemplate> resTemplateLocal = ObjectTemplate::New(isolate);
    resTemplateLocal->SetInternalFieldCount(1);
    resTemplateLocal->Set(String::NewFromUtf8(isolate, "end"), FunctionTemplate::New(isolate, resEnd));

    resTemplate.Reset(isolate, resTemplateLocal);

    NODE_SET_METHOD(exports, "setUserData", setUserData<uWS::SERVER>);
    NODE_SET_METHOD(exports, "getUserData", getUserData<uWS::SERVER>);
    NODE_SET_METHOD(exports, "clearUserData", clearUserData<uWS::SERVER>);
    NODE_SET_METHOD(exports, "getAddress", getAddress<uWS::SERVER>);

    NODE_SET_METHOD(exports, "transfer", transfer);
    NODE_SET_METHOD(exports, "upgrade", upgrade);
    NODE_SET_METHOD(exports, "connect", connect);
    NODE_SET_METHOD(exports, "setNoop", setNoop);
    registerCheck(isolate);
}

NODE_MODULE(uws, Main)
