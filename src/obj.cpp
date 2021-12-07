#include "obj.h"

void Obj::load(Json::Value& json) {
    (void)json;
}

void Obj::save(Json::Value& json) {
    (void)json;
}

void operator << (Obj& obj, Json::Value& json) {
    obj.load(json);
}

void operator >> (Obj& obj, Json::Value& json) {
    obj.save(json);
}
