#include <json\json.h>

#include "field_pick.h"

void FIELD_MANAGER::ParseinitialData(const Json::Value & data)
{
  /* field.resize(data["field"]["height"].asInt());
   for (int i = 0; i < field.size(); ++i) {
      field[i].resize(data["field"]["width"].asInt());
      for (int j = 0; j < field[i].size(); ++j) {
         field[i][j] = CELL_TYPE::space;
      }
   }
   for (unsigned int i = 0; i < data["obstruction"].size(); ++i) {
      field[data["obstruction"][i]["position"][0].asInt()][data["obstruction"][i]["position"][1].asInt()] = CELL_TYPE::barrier;
   }*/
   // fix from [][] to []
}

void FIELD_MANAGER::ParseData(const Json::Value & data)
{
   time = data["time"].asDouble();
   for (unsigned int i = 0; i < data["chips"].size(); ++i) {
      field.Get(data["chips"]["position"][0].asInt(), data["chips"]["position"][1].asInt()) = CELL_TYPE::chip;
   }
   units.clear();
   for (unsigned int i = 0; i < data["players"].size(); ++i) {
      units.push_back(UNIT(
         PNT(data["players"][i]["position"][0].asInt(),
         data["players"][i]["position"][1].asInt()),
         data["players"][i]["ID"].asInt()));
   }
}

Json::Value FIELD_MANAGER::GetData()
{
   return Json::Value();
}