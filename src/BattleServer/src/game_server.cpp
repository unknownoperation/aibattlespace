#include "game_base.h"
#include <Json/json.h>

#include "game_server.h"

void GAME_SERVER::InitSession (void)
{
   SetConnection();

   game = new GAME_BASE();
}

void GAME_SERVER::ReleaseSession (void)
{
   delete game;

   ReleaseConnection();
}

void GAME_SERVER::ServerLoop (void)
{
// while (true) {
      Json::Value json;

      game->RenderNextFrame();
      game->GetGameFrameJSON(json);

      SendGameFrameJSON(json);
// }
}
