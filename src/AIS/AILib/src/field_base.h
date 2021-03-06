#pragma once

#include <json/json.h>
///class for getting and parsing data in json format
class FIELD_BASE {
public:
   virtual void ParseinitialData(const Json::Value & data) = 0;
   virtual void ParseData(const Json::Value& data) = 0;
   virtual Json::Value GetData() = 0;
};
