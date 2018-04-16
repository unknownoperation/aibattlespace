#include <map>

#include "game_common.h"
#include "field.h"



DIRECTION GetDirEnum(const std::string & str)
{
   std::map<std::string, DIRECTION> directions;

   directions.insert(std::pair<std::string, DIRECTION>(std::string("UP"   ), DIRECTION::up   ));
   directions.insert(std::pair<std::string, DIRECTION>(std::string("RIGHT"), DIRECTION::right));
   directions.insert(std::pair<std::string, DIRECTION>(std::string("DOWN" ), DIRECTION::down ));
   directions.insert(std::pair<std::string, DIRECTION>(std::string("LEFT" ), DIRECTION::left ));
   directions.insert(std::pair<std::string, DIRECTION>(std::string("NONE" ), DIRECTION::none ));
   

   return (*(directions.find(str))).second;
}

std::string GetDirStr(const DIRECTION & dir)
{
   std::map<DIRECTION, std::string> directions;

   directions.insert(std::pair<DIRECTION, std::string>(DIRECTION::up, std::string("UP")));
   directions.insert(std::pair<DIRECTION, std::string>(DIRECTION::right, std::string("RIGHT")));
   directions.insert(std::pair<DIRECTION, std::string>(DIRECTION::down, std::string("DOWN")));
   directions.insert(std::pair<DIRECTION, std::string>(DIRECTION::left, std::string("LEFT")));
   directions.insert(std::pair<DIRECTION, std::string>(DIRECTION::none  ,std::string("NONE") ));


   return (*(directions.find(dir))).second;
}


UNIT::UNIT(PNT point, int id)
{
   x = point.x;
   y = point.y;
   id = id;
}


void UNIT::Move(DIRECTION dir, FIELD gameMap) // TODO check maybe need to invert Y
{
   PNT newCoord = PNT(this->x, this->y);
   switch (dir)
   {

   case DIRECTION::up:
      newCoord.y--;
      break;
   case DIRECTION::down:
      newCoord.y++;
      break;
   case DIRECTION::left:
      newCoord.x--;
      break;
   case DIRECTION::right:
      newCoord.x++;
      break;
   case DIRECTION::none:
      break;
   }
   if (!gameMap.isBarrier(newCoord)) // do movement if possible
   {
      this->x = newCoord.x;
      this->y = newCoord.y;
   }
}
