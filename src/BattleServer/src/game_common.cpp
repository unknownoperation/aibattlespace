#include <map>

#include "game_common.h"

DIRECTION GetDir(const std::string & str)
{
   std::map<std::string, DIRECTION> directions;

   directions.insert(std::pair<std::string, DIRECTION>(std::string("UP"   ), DIRECTION::up   ));
   directions.insert(std::pair<std::string, DIRECTION>(std::string("RIGHT"), DIRECTION::right));
   directions.insert(std::pair<std::string, DIRECTION>(std::string("DOWN" ), DIRECTION::down ));
   directions.insert(std::pair<std::string, DIRECTION>(std::string("LEFT" ), DIRECTION::left ));
   directions.insert(std::pair<std::string, DIRECTION>(std::string("NONE" ), DIRECTION::none ));
   

   return (*(directions.find(str))).second;
}
