#pragma once

#include <json/json.h>

class FIELD_BASE {
public:
   virtual void ParseinitialData(const Json::Value& data) = 0;
   virtual void ParseData(const Json::Value& data) = 0;
	 virtual Json::Value PackData(void) = 0; // Пакуем   
	 virtual Json::Value GetData() = 0;
};