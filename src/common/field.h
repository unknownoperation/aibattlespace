#pragma once

#include <vector>

#include "game_common.h"


void GenerateGameMap(int h, int w, FIELD & fld);

class FIELD {
public:
   friend void GenerateGameMap(int h, int w, FIELD & fld);

   CELL_TYPE   Get(int i, int j) const;
   CELL_TYPE & Get(int i, int j);

   bool isSpace   (PNT & point) { return Get(point.x, point.y) == CELL_TYPE::space; };
   bool isBarrier (PNT & point) { return Get(point.x, point.y) == CELL_TYPE::barrier; };
   void resize(int sz) { field.resize(sz); }
   size_t  size(void) { return field.size(); }


   std::vector<CELL_TYPE> field;
   int height;
   int width;
};
