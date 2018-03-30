#include <Windows.h>
#include <iostream>
#include "game_base.h"
#include "game_implementation.h"
#include <Json/json.h>

#include "game_server.h"

void GAME_SERVER::InitSession (void)
{
   SetConnection("tcp://127.0.0.1:8001");
   ai_connector.SetConnection("tcp://127.0.0.1:8000");

   game = new GAME_IMPLEMENTATION();
}

void GAME_SERVER::ReleaseSession (void)
{
   delete game;

   ReleaseConnection();
   ai_connector.ReleaseConnection();
}

void GAME_SERVER::ServerLoop (void)
{
	Json::Value init;

	game->GetInitialData(init);
	SendGameFrameJSON(init);
   ai_connector.SendData(init);
   init = ai_connector.ReceiveData();
   while (true) {
      Json::Value json;

      game->RenderNextFrame();
      game->GetGameFrameJSON(json);

      SendGameFrameJSON(json);
      ai_connector.SendData(json);
      json = ai_connector.ReceiveData();
      std::cout << "Received from AI" << std::endl << json.asString() << std::endl;
      Sleep(2000);
   }
}
