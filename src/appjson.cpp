#include "appjson.h"
#include "obj.h"
#include <fstream>

bool AppJson::loadFromFile(std::string fileName, Json::Value &jv) {
    std::ifstream doc(fileName, std::ifstream::binary);
    if (!doc.good()) {
        // GTRACE("doc(%s).good() return false", fileName.data());
        return false;
    }
    doc >> jv;
    return true;
}

bool AppJson::saveToFile(std::string fileName, Json::Value &jv) {
    std::ofstream doc(fileName, std::ofstream::binary);
    if (!doc.good()) {
        return false;
    }
    doc << jv;
    return true;
}

void operator >> (Json::Value& json, Obj& obj) {
    obj.load(json);
}

void operator << (Json::Value& json, Obj& obj) {
    obj.save(json);
}

void operator >> (Json::Value& json, int& i) {
    if (!json.isNull())
        i = json.asInt();
}

void operator << (Json::Value& json, int& i) {
    json = i;
}

void operator >> (Json::Value& json, std::string& s) {
    if (!json.isNull())
        s = json.asString();
}

void operator << (Json::Value& json, std::string& s) {
    json = s;
}
