#include <json/json.h>

#include "game_base.h"

void GAME_BASE::GetGameFrameJSON (Json::Value & scene)
{
   scene[0]["object"] = "Barrier";
   scene[0]["colour"] = 1;
   scene[1]["object"] = "Wall";
   scene[1]["colour"] = 6;
   scene[2]["object"] = "Stone";
   scene[2]["colour"] = 7;
}

void GAME_BASE::RenderNextFrame(void)
{
}
