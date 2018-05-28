#include <stdio.h>

#include "game_server.h"

int main (int argc, char ** argv) {

   GAME_SERVER server;

   printf("initializing\n");
   server.InitSession();
   printf("inited\n");

   std::string path("");

   if (argc == 3 && !strcmp(argv[1], "-p")) {
      path = path + std::string(argv[2]);
   }

   server.ServerLoop(path);

   /*server.ReleaseSession();*/

   printf("IMA SUPA SERVA\n");
   getchar();

   return 0;
}
