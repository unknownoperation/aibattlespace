#include <iostream>
#include <stdlib.h>
#include <vector>	
#include "player.h"
#include "field_pick.h"

PLAYER::PLAYER()
{
}

void PLAYER::run_game()
{
   FIELD_MANAGER * field = dynamic_cast<FIELD_MANAGER*>(GetField());

   while (GetGameStage() != GAME_STAGE::result) {
      GetData();
      std::vector<UNIT_RESPONSE> res;
      // Using player coordinates and map
      PNT curCoord = PNT(field->players[GetMyPlayerID()].x, field->players[GetMyPlayerID()].y);
      // find posible directions
      PNT coord;
      PNT possibleCoord[4];
      int possibleMove[4];
      PNT movements[4] = { PNT(0, 1), PNT(1, 0), PNT(0, -1) , PNT(-1, 0) };
      int it = 0;		//number of possible movements
      for (int i = 0; i < 4; ++i)
      {
         coord.x = curCoord.x + movements[i].x;
         coord.y = curCoord.y + movements[i].y;
         if (field->field.isSpace(coord)) {
            possibleCoord[it] = coord;
            possibleMove[it++] = i;
         }
      }

      // Random choose direction
      res.push_back(UNIT_RESPONSE{ 1, (DIRECTION)possibleMove[rand() % it] });
      SendData(res);
   }
}
