#include <json/json.h>
#include <algorithm>
#include <random>

#include "game_pick.h"
#include "field_generator.h"


PLAYER::PLAYER(std::vector<PNT> startPos, int id)
{
   score = 0;
   this->id = id;
   for (int i = 0; i < startPos.size(); ++i) // Units num = gived point num
   {
      units.push_back(UNIT(startPos[0], 0));
   }
}

GAME_PICK::GAME_PICK()
{
   std::vector<PNT> startPos1;
   std::vector<PNT> startPos2;
   // // Create game map
   GenerateGameMap(FIELD_SIZE, FIELD_SIZE, gameMap, startPos1, startPos2);
   // // Initialize players and their unit positions
   
   PLAYER player1 = PLAYER(startPos1, 1);
   players.push_back(player1);

   PLAYER player2 = PLAYER(startPos2, 2);
   players.push_back(player2);
   // Start timer
   // ...
   // Generate first chips
   GenerateChips();
   // set game stage
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

   static int aiID = 0;
   std::string key = std::to_string(aiID);
   int key_length = 8;
   std::random_device rd;
   std::mt19937 gen(rd());
   std::uniform_int_distribution<> dis(1, 10000);

   ++aiID;
   for(int i = 0; i < key_length - 1; ++i) {
      char new_symbol;
      if ((i & 1) == 0) {
         new_symbol = 48 + dis(gen) % 10;
      } else {
         new_symbol = 97 + dis(gen) % 26;
      }
      key += new_symbol;
   }
   data["key"] = key;
   
   data["colours"]["background"][0] = 135;
   data["colours"]["background"][1] = 206;
   data["colours"]["background"][2] = 235;

   data["colours"]["barrier"][0] = 0;
   data["colours"]["barrier"][1] = 0;
   data["colours"]["barrier"][2] = 0;

   data["colours"]["freespace"][0] = 102;
   data["colours"]["freespace"][1] = 205;
   data["colours"]["freespace"][2] = 170;

   data["colours"]["player_1"][0] = 255;
   data["colours"]["player_1"][1] = 0;
   data["colours"]["player_1"][2] = 0;

   data["colours"]["player_2"][0] = 0;
   data["colours"]["player_2"][1] = 0;
   data["colours"]["player_2"][2] = 255;

   data["colours"]["chips"][0] = 153;
   data["colours"]["chips"][1] = 50;
   data["colours"]["chips"][2] = 204;
}

void GAME_PICK::GetGameFrameJSON(Json::Value & scene) // TODO: use functions from json_manager
{
    scene.clear();

    scene["time"] = time;
    scene["game_stage"] = stages[(int)gameStage];

    for (int i = 0; i < chips.size(); i++) {
        scene["chips"][i]["position"][0] = chips[i].x;
        scene["chips"][i]["position"][1] = chips[i].y;
    }

    for (int i = 0; i < players.size(); i++) {
        scene["players"][i]["ID"] = players[i].GetId();
        scene["players"][i]["points"] = players[i].GetScore();

        for (int j = 0; j < players[i].units.size(); j++) {
            scene["players"][i]["position"][0] = players[i].units[j].x;
            scene["players"][i]["position"][1] = players[i].units[j].y;
        }
    }
}

std::vector<std::vector<DIRECTION>> GAME_PICK::ParseJsonFromAI (void) // TODO: check this
{
   std::vector<std::vector<DIRECTION>> moveDirs; // We have to get directions of movement of every unit of 2 players
   // Parse Jsons
   int playersNum = 2;
   moveDirs.resize(playersNum);
   for (unsigned int i = 0; i < playersNum; ++i)
   {
      moveDirs[i].push_back(GetDirEnum(jsonFromAi[i]["players"][0]["direction"].asCString()));
      printf("Direction %s\n", jsonFromAi[i]["players"][0]["direction"].asCString());
   } 
   return moveDirs;
}


void PLAYER::Move(std::vector<DIRECTION> dirs, FIELD gameMap)
{
   // Check if moveDirs num matches with units num
   assert(dirs.size() == units.size());
   for (int i = 0; i < units.size(); ++i)
      units[i].Move(dirs[i], gameMap); // Move if possible
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

typedef std::vector<PNT>::iterator POINT_ITERATOR; // ��������� � �����
typedef std::vector<PLAYER>::iterator PLAYER_ITERATOR; // ��������� � �����

double distance(PNT point1, PNT point2)
{
   return sqrt((point1.x - point2.x) * (point1.x - point2.x) + (point1.y - point2.y) * (point1.y - point2.y));
}

double EvalDistancesFromUnitsToPoint(std::vector<PLAYER> players, PNT point)
{
   double dist = 0.0;
   for (PLAYER player : players)
      for (UNIT unit : player.units)
      {
         dist += distance(PNT(unit.x, unit.y), point);
      }
   return dist;
}

void GAME_PICK::GenerateChips(void)
{
   if (chips.size() == CHIPS_NUM_DURING_GAME) // No need to add chips
      return;

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
   typedef std::vector<POINT_DISTANCE>::iterator POINT_DISTANCE_ITERATOR;
   for (POINT_DISTANCE_ITERATOR cell = freeCells.begin(); cell < freeCells.end(); ++cell)
   {
      (*cell).distance = EvalDistancesFromUnitsToPoint(players, (*cell).point); // Works
   }

   // Find the biggest distances
   std::sort(freeCells.begin(), freeCells.end(),
      [](POINT_DISTANCE first, POINT_DISTANCE second) { return first.distance > second.distance; });
   // Put chips in places with max distance 
   int numChipsToGenerate = CHIPS_NUM_DURING_GAME - chips.size();
   double chipsInUse = 0.8;
   int maxIndex = int(chipsInUse * freeCells.size());

   int numIter = 0;
   bool checkDistance = true;
   while (chips.size() < CHIPS_NUM_DURING_GAME)
   {
      int index = std::rand() % maxIndex;
      double minDist = 100.; // min distance from new chip to other chips
      if (checkDistance)// Want to make minDist > 1
      {
         for (int i = 0; i < chips.size() && minDist > 1.; ++i)
         {
            if (distance(chips[i], freeCells[index].point) <= 1.)
               minDist = 0.;
         }
         if (minDist > 1.)
            chips.push_back(freeCells[index].point);
      }
      else // If cannot put chips in places with distance > 1
      {
         if (std::find(chips.begin(), chips.end(), freeCells[index].point) == chips.end())
         {
            chips.push_back(freeCells[index].point);
         }
      }
      if (numIter > 20 * numChipsToGenerate) // ���� �� ����������� ���� �� ����� ���������� ����� �� ���������� ���� �� �����
         checkDistance = false;
      numIter++;
   }
}

void GAME_PICK::RenderNextFrame(void)
{
   // Draw game in console
   drawGame();

   if (getGameStage() == GAME_STAGE::result) {
      SetGameStage(GAME_STAGE::compliting);
      return;
   }
   // Get data from AI with JSON
   std::vector<std::vector<DIRECTION>> moveDirs = ParseJsonFromAI();

   if (players.size() > 1) {
      printf("Player 1 OLD pos x=%d y=%d\n", players[0].units[0].x, players[0].units[0].y);
      printf("Player 2 OLD pos x=%d y=%d\n", players[1].units[0].x, players[1].units[0].y);
   }

   // Move players
   for (int i = 0; i < moveDirs.size(); ++i)
      players[i].Move(moveDirs[i], gameMap);

   if (players.size() > 1) {
      printf("Player 1 NEW pos x=%d y=%d\n", players[0].units[0].x, players[0].units[0].y);
      printf("Player 2 NEW pos x=%d y=%d\n", players[1].units[0].x, players[1].units[0].y);
   }

   // Delete chips
   chips.erase(std::remove_if(chips.begin(), chips.end(), [&] (const PNT & pnt) {
      bool markForDelete = false;

      for (int i = 0; i < players.size(); ++i) {
         if (CheckIfReachedChip(players[i], pnt)) {
            markForDelete = true;
            players[i].IncScore(POINTS_PER_CHIP);
         }
      }

      return markForDelete; }), chips.end());


    printf("Player 1 score %d\n", players[0].GetScore());
    printf("Player 2 score %d\n", players[1].GetScore());

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

void GAME_PICK::drawGame(void)
{
   std::vector<PNT> printedTwoTimesCells;
   for (int j = gameMap.height - 1; j >= 0; j--)
   {
      for (int i = 0; i < gameMap.width; ++i)
      {
         int printed = 0;
         for (int k = 0; k < players.size(); ++k)
         {
            if (i == players[k].units[0].x && j == players[k].units[0].y)
            {
               if (!printed)
               {
                  printf(" %d", k + 1);
                  printed++;
               }
            }
         }
         for (int k = 0; k < chips.size(); ++k)
         {
            if (i == chips[k].x && j == chips[k].y)
            {
               printf(" *");
               printed++;
            }
         }
         if (gameMap.Get(i, j) == CELL_TYPE::barrier)
         {
            printf(" #");
            printed++;
         }
         if (!printed)
         {
            if (gameMap.Get(i, j) == CELL_TYPE::space)
            {
               printf("  ");
            }
         }

         if (printed > 1)
            printedTwoTimesCells.push_back(PNT(i, j));
      }
      printf("\n");
   }
   if (printedTwoTimesCells.size() > 0)
   {
      printf("Printed two times:\n");
      for (int i = 0; i < printedTwoTimesCells.size(); ++i)
      {
         printf("x=%d, y=%d | ", printedTwoTimesCells[i].x, printedTwoTimesCells[i].y);
      }
   }
}