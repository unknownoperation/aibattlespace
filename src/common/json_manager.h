#pragma once

#include <vector>
#include "game_common.h"
#include "json\json.h"

class JSON_MANAGER {
public:
	void ParseinitialData(const Json::Value& data);
	void ParseData(const Json::Value& data);
	Json::Value PackData(void);
	Json::Value GetData();
private:
	//std::vector<std::vector<CELL_TYPE>> field;// Why we need this?
	//std::vector<std::vector<CELL_TYPE>> basic_field; // Why we need this?
};