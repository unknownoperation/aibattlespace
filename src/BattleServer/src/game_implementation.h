#pragma once

#include <json/json.h>
#include "game_base.h"

class GAME_IMPLEMENTATION : public GAME_BASE {
public:
   GAME_IMPLEMENTATION() {}
   ~GAME_IMPLEMENTATION() {}

   void GetInitialData(Json::Value & data) override;
   void GetGameFrameJSON(Json::Value & scene) override;
   void RenderNextFrame(void) override;
private:
	Json::Value scene;
};
