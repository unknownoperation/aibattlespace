#include "player.h"

int main(void)
{
   PLAYER player("tcp://127.0.0.1:8000");

   player.run_game();
   return 0;
}