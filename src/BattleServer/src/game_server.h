#pragma once

#include "connector.h"

class GAME_BASE;

class GAME_SERVER : private CONNECTOR {
public:
   void InitSession    (void);
   void ReleaseSession (void);
   
   void ServerLoop (void);

private:
   GAME_BASE * game;
};
