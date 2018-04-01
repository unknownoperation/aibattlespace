#include <vector>
#include "player.h"
#include "field_implementation.h"

PLAYER::PLAYER(const std::string& url) :
   PLAYER_BASE(url)
{
}
void PLAYER::run_game()
{
   FIELD_IMPLEMENTATION* field = dynamic_cast<FIELD_IMPLEMENTATION*>(GetField());

   while (GetGameStage() != GAME_STAGE::compliting) {
      std::vector<UNIT_RESPONSE> res;

      res.push_back(UNIT_RESPONSE{ 1, DIRECTION::up });
      res.push_back(UNIT_RESPONSE{ 3, DIRECTION::right });
      SendData(res);
   }
}
