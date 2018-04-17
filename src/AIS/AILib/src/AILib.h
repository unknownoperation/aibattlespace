#pragma once

#include <string>
#include <vector>
#include "two_way_connector.h"
#include "game_common.h"

class FIELD_BASE;

class PLAYER_BASE : private TWO_WAY_CONNECTOR {
public:
   PLAYER_BASE();
   ~PLAYER_BASE();

   FIELD_BASE* GetField();
   void GetData();
   class UNIT_RESPONSE {
   public:
      int id;
      DIRECTION direction;
   };
   void SendData(std::vector<UNIT_RESPONSE> data);
   GAME_STAGE GetGameStage();
   int GetMyPlayerID();
private:
   std::string playerName;
   std::string serverAdress;
   FIELD_BASE *field;
   GAME_STAGE stage;
};
