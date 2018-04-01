#pragma once

#include <vector>
#include "field_base.h"
#include "game_common.h"


struct CELL {
    CELL_TYPE type;
    bool isVisited;
    int position;

    CELL(CELL_TYPE newType, int pos) : type(newType), isVisited(false), position(pos) {}
};

enum class CELL_TYPE {
	space = 0,
	barrier // Oganes deleted chip and added to elements of game by vector
};

class FIELD_IMPLEMENTATION {
public:
	 void GenerateGameMap(int height, int width);
	 bool isSpace(POINT point) { return field[point.x][point.y] == CELL_TYPE::space; };
	 bool isBarrier(POINT point) { return field[point.x][point.y] == CELL_TYPE::barrier; };
private:
  	std::vector<CELL_TYPE> field;
};