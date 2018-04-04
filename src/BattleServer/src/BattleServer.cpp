#include <stdio.h>

#include "game_server.h"

int main (void) {

   GAME_SERVER server;

   printf("initializing\n");
   server.InitSession();
   printf("inited\n");

   server.ServerLoop();

   server.ReleaseSession();

   printf("IMA SUPA SERVA\n");
   getchar();

   return 0;
}
