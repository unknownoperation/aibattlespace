#pragma once

#include <Json/json-forwards.h>

class GAME_BASE {
public:
   virtual ~GAME_BASE() {}

   virtual void GetInitialData(Json::Value & data) = 0;
   virtual void GetGameFrameJSON (Json::Value & scene) = 0;
   virtual void RenderNextFrame  (void) = 0;
};
