#include <Windows.h>
#include <iostream>
#include "game_base.h"
#include "game_implementation.h"
#include <Json/json.h>

#include "game_server.h"

void GAME_SERVER::InitSession (void)
{
   backend_connector.SetConnection("tcp://127.0.0.1:8001", TWO_WAY_CONNECTOR::CONNECTION_TYPE::bind);
   ai_connector[0].SetConnection("tcp://127.0.0.1:8000", TWO_WAY_CONNECTOR::CONNECTION_TYPE::bind);

   game = new GAME_IMPLEMENTATION();
}

void GAME_SERVER::ReleaseSession (void)
{
   delete game;

   backend_connector.ReleaseConnection();
   ai_connector[0].ReleaseConnection();
}

void GAME_SERVER::ServerLoop (void)
{
	Json::Value init;

	game->GetInitialData(init);
   backend_connector.SendData(init);
   ai_connector[0].SendData(init);
   init = ai_connector[0].ReceiveData();
   while (true) {
      Json::Value json;

      game->RenderNextFrame();
      game->GetGameFrameJSON(json);

      backend_connector.SendData(json);
      ai_connector[0].SendData(json);
      json = ai_connector[0].ReceiveData();
      std::cout << "Received from AI" << std::endl << Json::StyledWriter().write(json) << std::endl;
      Sleep(2000);
   }
}
