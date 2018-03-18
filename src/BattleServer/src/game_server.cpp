#include "game_base.h"
#include "game_implementation.h"
#include <Json/json.h>

#include "game_server.h"

void GAME_SERVER::InitSession (void)
{
   SetConnection("tcp://127.0.0.1:8000");

   game = new GAME_IMPLEMENTATION();
}

void GAME_SERVER::ReleaseSession (void)
{
   delete game;

   ReleaseConnection();
}

void GAME_SERVER::ServerLoop (void)
{
	Json::Value init;

	game->GetInitialData(init);
	SendGameFrameJSON(init);
// while (true) {
      Json::Value json;

      game->RenderNextFrame();
      game->GetGameFrameJSON(json);

      SendGameFrameJSON(json);
// }
}
