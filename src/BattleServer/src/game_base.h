#pragma once

#include <Json/json-forwards.h>

class GAME_BASE {
public:
   GAME_BASE  () {}
   ~GAME_BASE () {}


   void GetGameFrameJSON (Json::Value & scene);
   void RenderNextFrame  (void);
};

