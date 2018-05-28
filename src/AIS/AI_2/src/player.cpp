#include <iostream>
#include <stdlib.h>
#include <vector>	
#include "player.h"
#include "field_pick.h"

PLAYER::PLAYER()
{
}
/// class for representating information about map to AI
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
void getPathToChip(std::vector<DIRECTION> & directions, FIELD_MANAGER* field, GameMap gameMap, PNT curCoord, PNT goalChip);
PNT findNearestChip(PNT curCoord, std::vector<PNT>chips);
void getPath(std::vector<DIRECTION> & directions, GameMap gameMap, PNT goalChip, std::vector<PNT> wave);

///Main function of AI
void PLAYER::run_game()
{
	FIELD_MANAGER * field = dynamic_cast<FIELD_MANAGER*>(GetField());
	int WALL = 9999;


	///fill the game_map array
	GameMap gameMap(field->field.width, field->field.size());
	PNT point;
	for (int i = 0; i < field->field.width; ++i)
		for (int j = 0; j < field->field.height; ++j)
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
	PNT goalChip;
	///main game loop
	while (GetGameStage() != GAME_STAGE::compliting) {///main game loop
													  // Using player coordinates and map
		GetData();
		PNT curCoord = PNT(field->players[GetMyPlayerID()].x, field->players[GetMyPlayerID()].y);

		// Если дошли до конца траектории
		if (i == directions.size())
			if (goalChip.x = curCoord.x && goalChip.y == curCoord.y)
				needToFound = true;
			else
				printf("error traectory ended but unit is not on last point of traectory");

		if (needToFound) {
			goalChip = findNearestChip(curCoord, field->chips);
			getPathToChip(directions, field, gameMap, curCoord, goalChip);
			needToFound = false;
			i = 0;
		}

		// Если фишку съел другой игрок
		if (std::find(field->chips.begin(), field->chips.end(), goalChip) == field->chips.end())
			needToFound = true;

		// Send result
		std::vector<UNIT_RESPONSE> res;
		res.push_back(UNIT_RESPONSE{ 1, directions[i] });
		SendData(res);

		printf("\n\nMy pos x=%d,y=%d\n", curCoord.x, curCoord.y);
		printf("Directions go to %d %d, step=%d/%d\n", goalChip.x, goalChip.y, i, directions.size());
		printf("Chosen direction = %d\n", directions[i]);
		for (int j = 0; j < directions.size(); j++)
		{
			if (i == j)
				printf("|%d|", directions[j]);
			else
				printf("%d", directions[j]);
		}
		printf("\n");
		i++;
	}
}

///function for getting fastest path to the specific chip
void getPathToChip(std::vector<DIRECTION> & directions, FIELD_MANAGER* field, GameMap gameMap, PNT curCoord, PNT goalChip) {
	directions.clear();
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
	getPath(directions, gameMap, goalChip, wave);
}

void getPath(std::vector<DIRECTION> &directions, GameMap gameMap, PNT goalChip, std::vector<PNT> wave) {
	PNT movements[4] = { PNT(0, -1), PNT(1, 0),  PNT(0, 1), PNT(-1, 0) };
	PNT curCoord = goalChip;
	PNT nextCoord;
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
}

PNT findNearestChip(PNT curCoord, std::vector<PNT>chips) {
	int minDistance = abs(curCoord.x - chips[chips.size() - 1].x) + abs(curCoord.y - chips[chips.size() - 1].y);
	int index = 0;
	for (int d = chips.size() / 2; d < chips.size() - 1; ++d)
	{
		int curDist = abs(curCoord.x - chips[d].x) + abs(curCoord.y - chips[d].y);
		if (curDist < minDistance) {
			minDistance = curDist;
			index = d;
		}
	}
	return chips[index];
}