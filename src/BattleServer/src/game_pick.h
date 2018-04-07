#pragma once

#include <vector>
#include <json/json-forwards.h>

#include <assert.h> // delete in the end

#include "game_base.h"
#include "field.h"
#include "game_common.h"

const static int CHIPS_NUM_DURING_GAME = 4;
const static int WINNIG_SCORE = 3;
const static int FIELD_SIZE = 10;
const static int POINTS_PER_CHIP = 1;


class PLAYER {
public:
   PLAYER(std::vector<PNT> startPos, int id);
   void Move(std::vector<DIRECTION> dirs, FIELD gameMap);
   void IncScore(int addPoints) { score += addPoints; }
   int GetScore(void) { return score; }
   std::vector<UNIT> units;
private:
   int score;
   int id;
};

class GAME_IMPLEMENTATION : public GAME_BASE {
public:
   GAME_IMPLEMENTATION();
   ~GAME_IMPLEMENTATION() {}

   void GetInitialData(Json::Value & data) override;
   void GetGameFrameJSON(Json::Value & scene) override;
   void RenderNextFrame(void) override;
    void GenerateChips(void);
private:
   std::vector<std::vector<DIRECTION>> ParseJsonFromAI (void);

   FIELD gameMap;
   std::vector<PNT> chips;
   std::vector<PLAYER> players;
   std::vector<PLAYER> winners;
};
