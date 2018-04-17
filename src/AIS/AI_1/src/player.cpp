#include <iostream>
#include <stdlib.h>
#include <vector>	
#include "player.h"
#include "field_pick.h"

PLAYER::PLAYER() 
{
}

class GameMap {
public:
   GameMap(int val, int size) {
      width = val;
      map.resize(size);
   }
   std::vector<int> map;
   
   int width;
   void Set(PNT point, int value) {
      map[point.y*width + point.x] = value;
   }
   int Get(PNT point) {
      return map[point.y*width + point.x];
   }
};
std::vector<DIRECTION> getPathToChip(FIELD_MANAGER* field, GameMap gameMap, PNT curCoord, PNT goalChip);
PNT findNearestChip(PNT curCoord, std::vector<PNT>chips);
std::vector<DIRECTION> getPath(GameMap gameMap, PNT goalChip, std::vector<PNT> wave);

void PLAYER::run_game()
{
   FIELD_MANAGER * field = dynamic_cast<FIELD_MANAGER*>(GetField());
   int WALL = 9999;

   
   //fill the game_map array
   GameMap gameMap(field->field.width, field->field.size());
   PNT point;
   for (int i = 0; i < field->field.width; ++i)
	   for (int j = 0; j <  field->field.height; ++j)
	   {
		   point = PNT(i, j);
		   if (field->field.isBarrier(point))
            gameMap.Set(point, WALL);
		   else 
            gameMap.Set(point, -1);
		}

   std::vector<DIRECTION> directions;
	int i = 0;
	bool needToFound = true;	  
	while (GetGameStage() != GAME_STAGE::compliting) {
		// Using player coordinates and map
		GetData();
		PNT curCoord = PNT(field->players[0].x, field->players[0].y);
		PNT goalChip;
		if (needToFound) {
			goalChip = findNearestChip(curCoord, field->chips);
			directions = getPathToChip(field, gameMap, curCoord, goalChip);
         needToFound = false;
         i = 0;
		}
      if (std::find(field->chips.begin(), field->chips.end(), goalChip) == field->chips.end())
        needToFound = true;

		// Send result
		std::vector<UNIT_RESPONSE> res;
      printf("i = %d\n", i);
      printf("direction = %d\n", directions[i]);
		res.push_back(UNIT_RESPONSE{ 1, directions[i]});
      printf("i = %d\n", i);
		SendData(res);  
		i++;
      if (i == directions.size())
         needToFound = true;
   }
}


std::vector<DIRECTION> getPathToChip(FIELD_MANAGER* field, GameMap gameMap, PNT curCoord, PNT goalChip) {
	std::vector<PNT> wave;
	std::vector<PNT> oldWave;
	PNT movements[4] = { PNT(0, -1), PNT(1, 0),  PNT(0, 1), PNT(-1, 0) };
   PNT coord;
   bool isFounded = false;

	oldWave.push_back(curCoord);
	int nstep = 0;
   gameMap.Set(curCoord, nstep);
	while (oldWave.size() > 0 && !isFounded)
	{
		++nstep;
		wave.clear();
		for (std::vector<PNT>::iterator i = oldWave.begin(); i != oldWave.end(); ++i)
		{
			for (int d = 0; d < 4; ++d)
			{
				coord.x = i->x + movements[d].x;
				coord.y = i->y + movements[d].y;
				if (gameMap.Get(coord) == -1)
				{
					wave.push_back(coord);
               gameMap.Set(coord, nstep);
               if (coord.x == goalChip.x && coord.y == goalChip.y) {
                  isFounded = true;
                  break;
               }
				}
			}
         if (isFounded)
            break;
		}
		oldWave = wave;
	}
	// Get the path
   return getPath(gameMap, goalChip, wave);
}


std::vector<DIRECTION> getPath(GameMap gameMap, PNT goalChip, std::vector<PNT> wave) {
   PNT movements[4] = { PNT(0, -1), PNT(1, 0),  PNT(0, 1), PNT(-1, 0) };
   PNT curCoord = goalChip;
   PNT nextCoord;
   std::vector<DIRECTION> directions;
   int i = 0;
   wave.clear();
   wave.push_back(curCoord);
   while (gameMap.Get(curCoord) != 0)
   {
      for (int d = 0; d < 4; ++d)
      {
         nextCoord.x = curCoord.x + movements[d].x;
         nextCoord.y = curCoord.y + movements[d].y;
         if (gameMap.Get(curCoord) - 1 == gameMap.Get(nextCoord))
         {
            curCoord = nextCoord;
            directions.push_back(DIRECTION((d + 2) % 4));
            wave.push_back(curCoord);
            break;
         }
      }
   }

   std::reverse(directions.begin(), directions.end());
   return directions;
}

PNT findNearestChip(PNT curCoord, std::vector<PNT>chips) {
	int minDistance = (curCoord.x - chips[0].x) + (curCoord.y - chips[0].y);
	int index = 0;
	for (int d = 1; d < chips.size(); ++d)
	{
		if ((curCoord.x - chips[d].x) + (curCoord.y - chips[d].y) < minDistance) {
			minDistance = (curCoord.x - chips[d].x) + (curCoord.y - chips[d].y);
			index = d;
		}
	}
	return chips[index];
}