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

