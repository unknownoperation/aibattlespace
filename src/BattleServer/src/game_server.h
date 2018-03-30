#pragma once

#include "connector.h"
#include "two_way_connector.h" 

class GAME_BASE;

class GAME_SERVER : private CONNECTOR {
public:
   void InitSession    (void);
   void ReleaseSession (void);
   
   void ServerLoop (void);

private:
   GAME_BASE * game;
   TWO_WAY_CONNECTOR ai_connector;
};
