#include <json\json.h>

#include "field_pick.h"

void FIELD_MANAGER::ParseinitialData(const Json::Value & data)
{
   std::string tmp = Json::StyledWriter().write(data);
   field.resize(data["field"]["height"].asInt() * data["field"]["width"].asInt());
   field.width = data["field"]["width"].asInt();
   field.height = data["field"]["height"].asInt();
   for (unsigned int i = 0; i < data["obstruction"].size(); ++i) {
      field.Get(data["obstruction"][i]["position"][0].asInt(), data["obstruction"][i]["position"][1].asInt()) = CELL_TYPE::barrier;
   }
}

void FIELD_MANAGER::ParseData(const Json::Value & data)
{
   time = data["time"].asDouble();

   chips.clear();
   for (unsigned int i = 0; i < data["chips"].size(); ++i) {
	   chips.push_back(PNT(data["chips"][i]["position"][0].asInt(), data["chips"][i]["position"][1].asInt()));
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