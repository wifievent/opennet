#pragma once

#include "appjson.h"
#include "buf.h"
struct Obj {
    Obj() {}
    virtual ~Obj(){};

    std::string err_;
    Buf tempBuf_{nullptr, 0};

    virtual void load(Json::Value& json);
    virtual void save(Json::Value& json);
};

void operator << (Obj& obj, Json::Value& json);
void operator >> (Obj& obj, Json::Value& json);
