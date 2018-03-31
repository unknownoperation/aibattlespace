#pragma once

#include "two_way_connector.h" 

class GAME_BASE;

class GAME_SERVER {
public:
   void InitSession    (void);
   void ReleaseSession (void);
   
   void ServerLoop (void);

private:
   GAME_BASE * game;
   TWO_WAY_CONNECTOR ai_connector[2];
   TWO_WAY_CONNECTOR backend_connector;
};
