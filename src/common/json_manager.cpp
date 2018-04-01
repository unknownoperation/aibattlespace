#include "json_manager.h"

void JSON_MANAGER::ParseinitialData(const Json::Value & data)
{
	basic_field.resize(data["field"]["height"].asInt());
	for (int i = 0; i < basic_field.size(); ++i) {
		basic_field[i].resize(data["field"]["width"].asInt());
		for (int j = 0; j < basic_field[i].size(); ++j) {
			basic_field[i][j] = CELL_TYPE::space;
		}
	}
	for (int i = 0; i < data["obstruction"].size(); ++i) {
		basic_field[data["obstruction"][i]["position"][0].asInt()][data["obstruction"][i]["position"][1].asInt()] = CELL_TYPE::barrier;
	}
	field = basic_field;
}

void JSON_MANAGER::ParseData(const Json::Value & data)
{
	field = basic_field;
	time = data["time"].asDouble();
	for (int i = 0; i < data["chips"].size(); ++i) {
		field[data["chips"]["position"][0].asInt()][data["chips"]["position"][1].asInt()] = CELL_TYPE::chips;
	}
	units.clear();
	for (int i = 0; i < data["players"].size(); ++i) {
		units.push_back(UNIT{
			data["players"][i]["ID"].asInt(),
			data["players"][i]["position"][0].asInt(),
			data["players"][i]["position"][1].asInt() });
	}
}

Json::Value JSON_MANAGER::GetData()
{
	return Json::Value();
}