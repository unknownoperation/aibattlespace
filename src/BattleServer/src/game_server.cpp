#include <Windows.h>
#include <iostream>
#include <fstream>

#include "game_base.h"
#include "game_pick.h"
#include <Json/json.h>

#include "game_server.h"

void GAME_SERVER::InitSession (void)
{
   // backend_connector.SetConnection("tcp://127.0.0.1:8001", TWO_WAY_CONNECTOR::CONNECTION_TYPE::bind);
   ai_connector[0].SetConnection("tcp://127.0.0.1:8800", TWO_WAY_CONNECTOR::CONNECTION_TYPE::bind);

   game = new GAME_PICK();
}

void GAME_SERVER::ReleaseSession (void)
{
   delete game;

   // backend_connector.ReleaseConnection();
   ai_connector[0].ReleaseConnection();
}

void GAME_SERVER::ServerLoop (void)
{
   Json::Value init;

   game->SetGameStage(GAME_STAGE::starting); // Maybe delete one of connecting, starting?
   game->GetInitialData(init);

   // backend_connector.SendData(init);
   ai_connector[0].SendData(init);
   init = ai_connector[0].ReceiveData();
    
   Sleep(2000); // While showing window (not window in mean) with players name in frontend TODO 2000->const

   game->SetGameStage(GAME_STAGE::running);
   while (game->getGameStage() == GAME_STAGE::running) {

      Json::Value jsonFromServer;

      game->GetGameFrameJSON(jsonFromServer);

      // backend_connector.SendData(jsonFromServer);
      ai_connector[0].SendData(jsonFromServer);
      // ai_connector[1].SendData(jsonFromServer);

      Json::Value jsonFromAi1 = ai_connector[0].ReceiveData();
      std::cout << "Received from AI1" << std::endl << Json::StyledWriter().write(jsonFromAi1) << std::endl;
      game->SetJsonFromAi(jsonFromAi1, 0);

      // Json::Value jsonFromAi2 = ai_connector[1].ReceiveData();
      // std::cout << "Received from AI2" << std::endl << Json::StyledWriter().write(jsonFromAi2) << std::endl;
      // game->SetJsonFromAi(jsonFromAi2, 2);

      game->RenderNextFrame();

      std::ofstream ofstr("..\\..\\..\\src\\ClientBackend\\backendPart\\static\\backendPart\\objects.json");

      Json::StyledStreamWriter wrt;

      wrt.write(ofstr, jsonFromServer);

      ofstr.flush();

      Sleep(100);
   }

   // send json with result
   // ...
   Sleep(2000); // While showing results

   game->SetGameStage(GAME_STAGE::compliting);
   // send json with end game
   // ...
}
