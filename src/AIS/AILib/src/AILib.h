#pragma once

#include <string>
#include <vector>
#include "two_way_connector.h"
#include "game_common.h"

class FIELD_BASE;
struct PIMPL_PLAYER_BASE;
class UNIT_RESPONSE;

class PLAYER_BASE : private TWO_WAY_CONNECTOR {
public:
   PLAYER_BASE();
   ~PLAYER_BASE();

   class UNIT_RESPONSE {
   public:
      int id;
      DIRECTION direction;
   };

   FIELD_BASE* GetField();
   void GetData();
   void SendData(std::vector<UNIT_RESPONSE> data);
   GAME_STAGE GetGameStage();
   int GetMyPlayerID();
private:
   PIMPL_PLAYER_BASE * pimpl;
};
