#pragma once

#include <Json/json.h>
#include "game_common.h"

class GAME_BASE {
public:
   virtual ~GAME_BASE() {}
    
   virtual void GetInitialData(Json::Value & data) = 0;
   virtual void GetGameFrameJSON (Json::Value & scene) = 0;
   virtual void RenderNextFrame  (void) = 0;
    void SetJsonFromAi(Json::Value & jsonFromAi, int id) { this->jsonFromAi[id] = jsonFromAi; };
    void SetGameStage(GAME_STAGE gameStage) { this->gameStage = gameStage; };
    GAME_STAGE getGameStage(void) { return gameStage; }
    double time;
protected:
    Json::Value jsonFromAi[2];
    GAME_STAGE gameStage;
};
