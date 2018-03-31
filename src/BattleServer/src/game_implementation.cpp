#include "game_implementation.h"

void GAME_IMPLEMENTATION::GetInitialData(Json::Value & data)
{
   data["field"]["height"] = 10;
   data["field"]["width"] = 10;

   data["obstruction"][0]["type"] = "barrier";
   data["obstruction"][0]["position"][0] = 1;
   data["obstruction"][0]["position"][1] = 1;
   data["obstruction"][1]["type"] = "barrier";
   data["obstruction"][1]["position"][0] = 1;
   data["obstruction"][1]["position"][1] = 2;
   data["obstruction"][2]["type"] = "barrier";
   data["obstruction"][2]["position"][0] = 1;
   data["obstruction"][2]["position"][1] = 4;
   data["obstruction"][3]["type"] = "barrier";
   data["obstruction"][3]["position"][0] = 1;
   data["obstruction"][3]["position"][1] = 6;
   data["obstruction"][4]["type"] = "barrier";
   data["obstruction"][4]["position"][0] = 1;
   data["obstruction"][4]["position"][1] = 7;
   data["obstruction"][5]["type"] = "barrier";
   data["obstruction"][5]["position"][0] = 1;
   data["obstruction"][5]["position"][1] = 8;
   data["obstruction"][6]["type"] = "barrier";
   data["obstruction"][6]["position"][0] = 2;
   data["obstruction"][6]["position"][1] = 1;
   data["obstruction"][7]["type"] = "barrier";
   data["obstruction"][7]["position"][0] = 2;
   data["obstruction"][7]["position"][1] = 2;
   data["obstruction"][8]["type"] = "barrier";
   data["obstruction"][8]["position"][0] = 3;
   data["obstruction"][8]["position"][1] = 4;
   data["obstruction"][9]["type"] = "barrier";
   data["obstruction"][9]["position"][0] = 3;
   data["obstruction"][9]["position"][1] = 6;

   data["key"] = "57fa30ff";
}

void GAME_IMPLEMENTATION::GetGameFrameJSON(Json::Value & scene)
{
	scene = this->scene;
}

void GAME_IMPLEMENTATION::RenderNextFrame(void)
{
   scene["time"] = 0;
   scene["game_stage"] = "running";

	scene["chips"][0]["position"][0] = 0;
	scene["chips"][0]["position"][1] = 3;
	scene["chips"][1]["position"][0] = 7;
	scene["chips"][1]["position"][1] = 7;
	scene["chips"][2]["position"][0] = 9;
	scene["chips"][2]["position"][1] = 3;
	scene["chips"][3]["position"][0] = 0;
	scene["chips"][3]["position"][1] = 9;
	scene["chips"][4]["position"][0] = 5;
	scene["chips"][4]["position"][1] = 0;

	scene["players"][0]["ID"] = 1;
	scene["players"][0]["position"][0] = 5;
	scene["players"][0]["position"][1] = 0;
	scene["players"][1]["ID"] = 2;
	scene["players"][1]["position"][0] = 5;
	scene["players"][1]["position"][1] = 6;
}
