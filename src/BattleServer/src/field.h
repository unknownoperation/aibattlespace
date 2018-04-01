#pragma once

#include <vector>

#include "game_common.h"


class FIELD {
public:
   CELL_TYPE Get (int i, int j) const;
   void GenerateGameMap(int h, int w);
   bool isSpace   (PNT point) { return Get(point.x, point.y) == CELL_TYPE::space; };
   bool isBarrier (PNT point) { return Get(point.x, point.y) == CELL_TYPE::barrier; };
private:
   std::vector<CELL_TYPE> field;
   int height;
   int width;
};
