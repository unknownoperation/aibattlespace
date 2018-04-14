#include <json/json.h>
#include <algorithm>

#include "game_pick.h"
#include "field_generator.h"


PLAYER::PLAYER(std::vector<PNT> startPos, int id)
{
   score = 0;
   this->id = id;
   for (int i = 0; i < startPos.size(); ++i) // Units num = gived point num
   {
      units.push_back(UNIT(startPos[i], i));
   }
}

GAME_PICK::GAME_PICK()
{
   // // Create game map
   GenerateGameMap(FIELD_SIZE, FIELD_SIZE, gameMap);
   // // Initialize players and their unit positions
   std::vector<PNT> startPos1{ PNT(1, 1) }; // 1 1 move to define
   PLAYER player1 = PLAYER(startPos1, 1);
   players.push_back(player1);
   //
   // std::vector<PNT> startPos2{ PNT(9, 9) }; // 9 9 move to define
   // PLAYER player2 = PLAYER(startPos2, 2);
   // players.push_back(player2);
   // // Start timer
   // // ...
   // // Generate chips maybe not ?
   // GenerateChips();
   // // set game stage
   gameStage = GAME_STAGE::connecting;
}

void GAME_PICK::GetInitialData(Json::Value & data)  // TODO: use functions from json_manager
{
   data["field"]["height"] = gameMap.height;
   data["field"]["width"] = gameMap.width;

   int gameMapSize = gameMap.height * gameMap.width;
   for (int i = 0, j = 0; i < gameMapSize; i++)
   {
      if (gameMap.Get(i, 0) == CELL_TYPE::barrier) {
         data["obstruction"][j]["type"] = "barrier";
         data["obstruction"][j]["position"][0] = i % gameMap.width;
         data["obstruction"][j]["position"][1] = i / gameMap.width;
         j++;
      }
   }

   data["key"] = "57fa30ff";
}

void GAME_PICK::GetGameFrameJSON(Json::Value & scene) // TODO: use functions from json_manager
{
   /*scene["time"] = time;
   scene["game_stage"] = stages[gameStage];

   for (int i = 0; i < chips.size(); ++i)
   {
      scene["chips"][i]["position"][0] = chips[i].x;
      scene["chips"][i]["position"][1] = chips[i].y;
   }*/

   /////////////////////////////////////////////////////////////////////////
   // temporary

   static std::string jsn1("{             \
      \"time\": 12345,               \
                                   \
      \"game_stage\" : \"running\",    \
                                   \
      \"chips\" :                    \
      [                            \
   {                               \
      \"position\":[0, 3]            \
   },                              \
        {                          \
           \"position\":[7, 7]       \
        },                         \
        {                          \
           \"position\":[9, 3]       \
        },                         \
        {                          \
           \"position\":[0, 9]       \
        },                         \
        {                          \
           \"position\":[5, 0]       \
        }                          \
      ],                           \
                                   \
                                   \
      \"players\":                   \
      [                            \
      {                            \
         \"ID\" : 1,                 \
            \"position\" : [5, 0],   \
            \"points\" : 30          \
      },                           \
        {                          \
           \"ID\" : 2,               \
           \"position\" : [5, 6],    \
           \"points\" : 5            \
        }                          \
      ]                            \
}                                  ");

   static std::string jsn2("{             \
      \"time\": 12345,               \
                                   \
      \"game_stage\" : \"running\",    \
                                   \
      \"chips\" :                    \
      [                            \
   {                               \
      \"position\":[0, 3]            \
   },                              \
        {                          \
           \"position\":[0, 0]       \
        },                         \
        {                          \
           \"position\":[1, 1]       \
        },                         \
        {                          \
           \"position\":[2, 2]       \
        },                         \
        {                          \
           \"position\":[3, 3]       \
        }                          \
      ],                           \
                                   \
                                   \
      \"players\":                   \
      [                            \
      {                            \
         \"ID\" : 1,                 \
            \"position\" : [5, 5],   \
            \"points\" : 35          \
      },                           \
        {                          \
           \"ID\" : 2,               \
           \"position\" : [5, 6],    \
           \"points\" : 5            \
        }                          \
      ]                            \
}                                  ");

   static bool state = true;

   scene.clear();
   Json::Reader rd;

   if (state) {
      rd.parse(jsn1, scene);
   }  else{
      rd.parse(jsn2, scene);
   }
   state = !state;
}

std::vector<std::vector<DIRECTION>> GAME_PICK::ParseJsonFromAI (void) // TODO: check this
{
   std::vector<std::vector<DIRECTION>> moveDirs; // We have to get directions of movement of every unit of 2 players
   // Parse Jsons
   // AI1
   moveDirs.resize(2);
   std::string tmp = Json::StyledWriter().write(jsonFromAi[0]);
   for (unsigned int i = 0; i < jsonFromAi[0]["players"].size(); ++i)
   {
      moveDirs[i].push_back(GetDirEnum(jsonFromAi[0]["players"][i]["direction"].asCString()));
		printf("Direction %s\n", jsonFromAi[0]["players"][i]["direction"].asCString());
   }
	 
   // AI2
   /* for (unsigned int i = 0; i < jsonFromAi[1]["players"].size(); ++i)
   {
      moveDirs[i].push_back(GetDirEnum(jsonFromAi[1]["players"][i]["direction"].asCString()));
   } */
   return moveDirs;
}


void PLAYER::Move(std::vector<DIRECTION> dirs, FIELD gameMap)
{
   // Check if moveDirs num matches with units num
   assert(dirs.size() == units.size());
   for (int i = 0; i < dirs.size(); ++i)
   {
      units[i].Move(dirs[i], gameMap); // Move if possible
   }
}

bool CheckIfMatchesCoordinates(UNIT unit, PNT chip)
{
   return unit.x == chip.x && unit.y == chip.y;
}

bool CheckIfReachedChip(PLAYER player, PNT chip)
{
   for (UNIT unit : player.units)
   {
      if (CheckIfMatchesCoordinates(unit, chip))
         return true;
   }
   return false;
}

typedef std::vector<PNT>::iterator POINT_ITERATOR; // Перенести в Ашник
typedef std::vector<PLAYER>::iterator PLAYER_ITERATOR; // Перенести в Ашник

double EvalDistancesFromUnitsToPoint(std::vector<PLAYER> players, PNT point)
{
   double dist = 0.0;
   for (PLAYER player : players)
      for (UNIT unit : player.units)
      {
         dist += (unit.x - point.x) * (unit.x - point.x) + (unit.y - point.y) * (unit.y - point.y);
      }
   return dist;
}

void GAME_PICK::GenerateChips(void)
{
   struct POINT_DISTANCE
   {
      PNT point;
      double distance;
   };

   // get vector of free cells
   std::vector<POINT_DISTANCE> freeCells;
   for (int i = 0; i < FIELD_SIZE; ++i)
      for (int j = 0; j < FIELD_SIZE; ++j)
      {
         POINT_DISTANCE cell = POINT_DISTANCE{ PNT(i, j), 0.0 };
         if (gameMap.isSpace(cell.point))
            freeCells.push_back(cell);
      }

   // Compute distance from units of every player to free cells
   for (POINT_DISTANCE cell : freeCells)
   {
      cell.distance = EvalDistancesFromUnitsToPoint(players, cell.point); // Not works
   }
   typedef std::vector<POINT_DISTANCE>::iterator POINT_DISTANCE_ITERATOR;
   for (POINT_DISTANCE_ITERATOR cell = freeCells.begin(); cell < freeCells.end(); ++cell)
   {
      (*cell).distance = EvalDistancesFromUnitsToPoint(players, (*cell).point); // Works
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

void GAME_PICK::RenderNextFrame(void)
{
   // Get data from AI with JSON
   std::vector<std::vector<DIRECTION>> moveDirs = ParseJsonFromAI();

	 printf("Player 1 OLD pos x=%d y=%d\n", players[0].units[0].x, players[0].units[0].y);

   // Move players
   for (int i = 0; i < players.size(); ++i)
      players[i].Move(moveDirs[i], gameMap);

	 printf("Player 1 NEW pos x=%d y=%d\n", players[0].units[0].x, players[0].units[0].y);

   // Check for reaching chip by player
   for (int i = 0; i < players.size(); ++i)
      for (POINT_ITERATOR chip = chips.begin(); chip < chips.end(); ++chip)
         if (CheckIfReachedChip(players[i], *chip))
         {
            players[i].IncScore(POINTS_PER_CHIP);
            chips.erase(chip); // delete reached chip from chips
         }

	 printf("Player 1 score %d\n", players[0].GetScore());

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
