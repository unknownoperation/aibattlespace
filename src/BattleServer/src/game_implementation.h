#pragma once

#include <vector>
#include <json/json.h>
#include <algorithm>
#include <assert.h> // delete in the end
#include "game_base.h"
#include "field_implementation.h"
#include "game_common.h"

const static int CHIPS_NUM_DURING_GAME = 4;
const static int WINNIG_SCORE = 3;
const static int FIELD_SIZE = 10;
const static int POINTS_PER_CHIP = 1;

class UNIT : public POINT {
public:
	UNIT(POINT point, int id);
	void Move(DIRECTION dir, FIELD_IMPLEMENTATION gameMap);
private:
	int id;
};

class PLAYER {
public:
	PLAYER(std::vector<POINT> startPos, int id);
	void Move(std::vector<DIRECTION> dirs, FIELD_IMPLEMENTATION gameMap);
	void IncScore(int addPoints) { score += addPoints; }
	int GetScore(void) { return score; }
	std::vector<UNIT> units;
private:
	int score;
	int id;
};

class GAME_IMPLEMENTATION : public GAME_BASE {
public:
   GAME_IMPLEMENTATION() {}
   ~GAME_IMPLEMENTATION() {}

   void GetInitialData(Json::Value & data) override;
   void GetGameFrameJSON(Json::Value & scene) override;
   void RenderNextFrame(void) override;
	 void GenerateChips(void);
private:
	FIELD_IMPLEMENTATION gameMap;
	std::vector<POINT> chips;
	std::vector<PLAYER> players;
	std::vector<PLAYER> winners;
};
