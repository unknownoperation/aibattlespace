#include "game_implementation.h"

UNIT::UNIT(POINT point, int id)
{
	x = point.x;
	y = point.y;
	id = id;
}

PLAYER::PLAYER(std::vector<POINT> startPos, int id)
{
	score = 0;
	this->id = id;
	for (int i = 0; i < startPos.size(); ++i) // Units num = gived point num
	{
		units.push_back(UNIT(startPos[i], i));
	}
}

GAME_IMPLEMENTATION::GAME_IMPLEMENTATION()
{
	// Create game map
	gameMap.GenerateGameMap(FIELD_SIZE);
	// Initialize players and their unit positions
	std::vector<POINT> startPos1{ POINT(1, 1) }; // 1 1 move to define
	PLAYER player1 = PLAYER(startPos1, 1);
	players.push_back(player1);

	std::vector<POINT> startPos2{ POINT(9, 9) }; // 9 9 move to define
	PLAYER player2 = PLAYER(startPos2, 2);
	players.push_back(player2);
	// Start timer
	// ...
	// Generate chips maybe not ?
	GenerateChips();
	// set game stage
	gameStage = GAME_STAGE::connecting;
}

void GAME_IMPLEMENTATION::GetInitialData(Json::Value & data)  // TODO: use functions from json_manager
{
   data["field"]["height"] = 10;
   data["field"]["width"] = 10;

   data["obstruction"][0]["type"] = "barrier";
   data["obstruction"][0]["position"][0] = 1;
   data["obstruction"][0]["position"][1] = 1;
   data["obstruction"][1]["type"] = "barrier";
   data["obstruction"][1]["position"][0] = 1;
   data["obstruction"][1]["position"][1] = 2;
   data["obstruction"][2]["type"] = "barrier";
   data["obstruction"][2]["position"][0] = 1;
   data["obstruction"][2]["position"][1] = 4;
   data["obstruction"][3]["type"] = "barrier";
   data["obstruction"][3]["position"][0] = 1;
   data["obstruction"][3]["position"][1] = 6;
   data["obstruction"][4]["type"] = "barrier";
   data["obstruction"][4]["position"][0] = 1;
   data["obstruction"][4]["position"][1] = 7;
   data["obstruction"][5]["type"] = "barrier";
   data["obstruction"][5]["position"][0] = 1;
   data["obstruction"][5]["position"][1] = 8;
   data["obstruction"][6]["type"] = "barrier";
   data["obstruction"][6]["position"][0] = 2;
   data["obstruction"][6]["position"][1] = 1;
   data["obstruction"][7]["type"] = "barrier";
   data["obstruction"][7]["position"][0] = 2;
   data["obstruction"][7]["position"][1] = 2;
   data["obstruction"][8]["type"] = "barrier";
   data["obstruction"][8]["position"][0] = 3;
   data["obstruction"][8]["position"][1] = 4;
   data["obstruction"][9]["type"] = "barrier";
   data["obstruction"][9]["position"][0] = 3;
   data["obstruction"][9]["position"][1] = 6;

   data["key"] = "57fa30ff";
}

void GAME_IMPLEMENTATION::GetGameFrameJSON(Json::Value & scene) // TODO: use functions from json_manager
{
	scene["time"] = time;
	scene["game_stage"] = stages[gameStage];

	scene["chips"][0]["position"][0] = 0;
	scene["chips"][0]["position"][1] = 3;
	scene["chips"][1]["position"][0] = 7;
	scene["chips"][1]["position"][1] = 7;
	scene["chips"][2]["position"][0] = 9;
	scene["chips"][2]["position"][1] = 3;
	scene["chips"][3]["position"][0] = 0;
	scene["chips"][3]["position"][1] = 9;
	scene["chips"][4]["position"][0] = 5;
	scene["chips"][4]["position"][1] = 0;

	scene["players"][0]["ID"] = 1;
	scene["players"][0]["position"][0] = 5;
	scene["players"][0]["position"][1] = 0;
	scene["players"][1]["ID"] = 2;
	scene["players"][1]["position"][0] = 5;
	scene["players"][1]["position"][1] = 6;
}

std::vector<std::vector<DIRECTION>> ParseJsonFromAI(Json::Value jsonFromAi[]) // TODO: check this
{
	std::vector<std::vector<DIRECTION>> moveDirs; // We have to get directions of movement of every unit of 2 players
	// Parse Jsons
	// AI1
	for (int i = 0; i < jsonFromAi[0]["AI"].size(); ++i)
	{
		moveDirs[i].push_back(jsonFromAi[0]["AI"][i]["direction"].asCString);
	}

	// AI2
	for (int i = 0; i < jsonFromAi[1]["AI"].size(); ++i)
	{
		moveDirs[i].push_back(jsonFromAi[1]["AI"][i]["direction"].asCString);
	}
	return moveDirs;
}

void UNIT::Move(DIRECTION dir, FIELD_IMPLEMENTATION gameMap) // TODO check maybe need to invert Y
{
	POINT newCoord = POINT(this->x, this->y);
	switch (dir)
	{

	case directions[DIRECTION::up]:
		newCoord.y++;
		break;
	case directions[DIRECTION::down]:
		newCoord.y--;
		break;
	case directions[DIRECTION::left]:
		newCoord.x--;
		break;
	case directions[DIRECTION::right]:
		newCoord.x++;
		break;
	case directions[DIRECTION::none]:
		break;
	}
	if (!gameMap.isBarrier(newCoord)) // do movement if possible
	{
		this->x = newCoord.x; // Refactor
		this->y = newCoord.y;
	}
}

void PLAYER::Move(std::vector<DIRECTION> dirs, FIELD_IMPLEMENTATION gameMap)
{
	// Check if moveDirs num matches with units num
	assert(dirs.size() == units.size());
	for (int i = 0; i < dirs.size(); ++i)
	{
		units[i].Move(dirs[i], gameMap); // Move if possible
	}
}

bool CheckIfMatchesCoordinates(UNIT unit, POINT chip)
{
	return unit.x == chip.x && unit.y == chip.y;
}

bool CheckIfReachedChip(PLAYER player, POINT chip)
{
	for (UNIT unit : player.units)
	{
		if (CheckIfMatchesCoordinates(unit, chip))
			return true;
	}
	return false;
}

typedef std::vector<POINT>::iterator POINT_ITERATOR; // ��������� � �����
typedef std::vector<PLAYER>::iterator PLAYER_ITERATOR; // ��������� � �����

double EvalDistancesFromUnitsToPoint(std::vector<PLAYER> players, POINT point)
{
	double dist = 0.0;
	for (PLAYER player : players)
		for (UNIT unit : player.units)
		{
			dist += (unit.x - point.x) * (unit.x - point.x) + (unit.y - point.y) * (unit.y - point.y);
		}
	return dist;
}

void GAME_IMPLEMENTATION::GenerateChips(void)
{
	struct POINT_DISTANCE
	{
		POINT point;
		double distance;
	};

	// get vector of free cells
	std::vector<POINT_DISTANCE> freeCells;
	for (int i = 0; i < FIELD_SIZE; ++i)
		for (int j = 0; j < FIELD_SIZE; ++j)
		{
			POINT_DISTANCE cell = POINT_DISTANCE{ POINT(i, j), 0.0 };
			if (gameMap.isSpace(cell.point))
				freeCells.push_back(cell);
		}
	// Compute distance from units of every player to free cells
	for (POINT_DISTANCE cell : freeCells)
	{
		cell.distance = EvalDistancesFromUnitsToPoint(players, cell.point);
	}
	// Find the biggest distances
	std::sort(freeCells.begin(), freeCells.end(),
		[](POINT_DISTANCE first, POINT_DISTANCE second) { return first.distance > second.distance; });
	// Put chips in places with max distance 
	// TODO this algorithm put all new chips near each other
	int numChipsToGenerate = CHIPS_NUM_DURING_GAME - chips.size();
	for (int i = 0; i < numChipsToGenerate; ++i)
		chips.push_back(freeCells[i].point);
}

void GAME_IMPLEMENTATION::RenderNextFrame(void)
{
	// Get data from AI with JSON
	std::vector<std::vector<DIRECTION>> moveDirs = ParseJsonFromAI(jsonFromAi);

	// Move players
	for (int i = 0; i < players.size(); ++i)
		players[i].Move(moveDirs[i], gameMap);

	// Check for reaching chip by player
	for (int i = 0; i < players.size(); ++i)
		for (POINT_ITERATOR chip = chips.begin(); chip < chips.end(); ++chip)
			if (CheckIfReachedChip(players[i], *chip))
			{
				players[i].IncScore(POINTS_PER_CHIP);
				chips.erase(chip); // delete reached chip from chips
			}

	// Check if there are winners
	for (PLAYER_ITERATOR player = players.begin(); player < players.end(); ++player)
	{
		if ((*player).GetScore() == WINNIG_SCORE)
			winners.push_back(*player);
	}

	// If we have one or more winners end game
	if (winners.size() > 0) 
	{
		SetGameStage(GAME_STAGE::result);
		return;
	}

	// Generate chips
	GenerateChips();
}
