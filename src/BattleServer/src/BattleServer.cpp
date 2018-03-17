#include <stdio.h>

#include "game_server.h"

int main (void) {

   GAME_SERVER server;

   server.InitSession();

   server.ServerLoop();

   server.ReleaseSession();

   printf("IMA SUPA SERVA\n");
   getchar();

   return 0;
}