#include <time.h>
#include <stack>
#include <iostream>
#include <assert.h>

#include "field.h"




CELL_TYPE FIELD::Get (int i, int j) const
{
   assert(i + j * width < field.size());
   return field[i + j * width];
}


CELL_TYPE & FIELD::Get (int i, int j)
{
   assert(i + j * width < field.size());
   return field[i + j * width];
}

bool FIELD::isSpace(PNT & point)
{
   if (point.x < 0 || point.y < 0 || point.x >= width || point.y >= height)
      return false;
   return Get(point.x, point.y) == CELL_TYPE::space;
}

bool FIELD::isBarrier(PNT & point)
{
   if (point.x < 0 || point.y < 0 || point.x >= width || point.y >= height)
      return false;
   return Get(point.x, point.y) == CELL_TYPE::barrier;
}

