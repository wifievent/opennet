#pragma once

#include "json.h"

struct AppJson {
    static bool loadFromFile(std::string fileName, Json::Value &json);
    static bool saveToFile(std::string fileName, Json::Value &json);
};

struct Obj;
void operator >> (Json::Value& json, Obj& obj);
void operator << (Json::Value& json, Obj& obj);

void operator >> (Json::Value& json, int& i);
void operator << (Json::Value& json, int& i);

void operator >> (Json::Value& json, std::string& s);
void operator << (Json::Value& json, std::string& s);
