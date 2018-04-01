#pragma once
#include <vector>

   enum DIRECTION {
      up = 0,
      right,
      down,
      left,
			none
   };
   const std::vector<std::string> directions = {
		 "UP",
		 "RIGHT",
		 "DOWN",
		 "LEFT",
		 "NONE"
   };

   enum GAME_STAGE {
      connecting = 0,
      starting,
      running,
      result,
      compliting
   };
   const std::vector<std::string> stages{
      "connecting",
      "starting",
      "running",
      "result",
      "compliting"
   };

	 class POINT {
	 public:
		 POINT();
		 POINT(int x, int y) { this->x = x; this->y = y; }
		 int x, y;
	 };

	 enum CELL_TYPE {
		 space = 0,
		 barrier // Oganes deleted chip and added to elements of game by vector
	 };