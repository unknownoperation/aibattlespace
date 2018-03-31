#pragma once

#include <string>
#include <vector>
#include "two_way_connector.h"

class FIELD_BASE;

class PLAYER_BASE : private TWO_WAY_CONNECTOR {
public:
   PLAYER_BASE(const std::string& serverAdress);
   ~PLAYER_BASE();

   FIELD_BASE* GetField();
   void GetData();
   enum class DIRECTION {
      up = 0,
      right,
      down,
      left
   };
   const std::vector<std::string> directions = {
   "UP",
   "RIGHT",
   "DOWN",
   "LEFT"
   };
   class UNIT_RESPONSE {
   public:
      int id;
      DIRECTION direction;
   };
   void SendData(std::vector<UNIT_RESPONSE> data);
   enum class GAME_STAGE {
      connecting = 0,
      starting,
      running,
      result,
      compliting
   };
   const std::vector<std::string> stages{
      "connecting",
      "starting",
      "running",
      "result",
      "compliting"
   };
   GAME_STAGE GetGameStage();
private:
   std::string serverAdress;
   std::string playerName;
   FIELD_BASE *field;
   GAME_STAGE stage;
};
