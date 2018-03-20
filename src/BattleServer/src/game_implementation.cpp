#include "game_implementation.h"

void GAME_IMPLEMENTATION::GetInitialData(Json::Value & data)
{
	data[0]["object"] = "Barrier";
	data[0]["colour"] = 1;
	data[1]["object"] = "Wall";
	data[1]["colour"] = 6;
	data[2]["object"] = "Stone";
	data[2]["colour"] = 7;
}

void GAME_IMPLEMENTATION::GetGameFrameJSON(Json::Value & scene)
{
	scene = this->scene;
}

void GAME_IMPLEMENTATION::RenderNextFrame(void)
{
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
