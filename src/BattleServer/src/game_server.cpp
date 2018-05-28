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
   ai_connector[0].SetConnection("tcp://127.0.0.1:8800");
   ai_connector[1].SetConnection("tcp://127.0.0.1:8801");

   game = new GAME_PICK();
}

void GAME_SERVER::ReleaseSession (void)
{
   delete game;

   // backend_connector.ReleaseConnection();
   ai_connector[0].ReleaseConnection();
   ai_connector[1].ReleaseConnection();
}

void GAME_SERVER::ServerLoop (const std::string & path)
{
   Json::Value init;

   game->SetGameStage(GAME_STAGE::starting); // Maybe delete one of connecting, starting?
   game->GetInitialData(init);                                          
   ai_connector[0].SendData(init);
   init.clear();
   game->GetInitialData(init);
   ai_connector[1].SendData(init);
   // backend_connector.SendData(init);
   
   {
      std::cout << (path + std::string("game_map .json")) << std::endl;
       std::ofstream ofstr(path + std::string("game_map .json"));
       std::cout << path << "path";
       Json::StyledStreamWriter wrt;

       wrt.write(ofstr, init);
   }

   // backend_connector.SendData(init);
   init = ai_connector[0].ReceiveData();
   init = ai_connector[1].ReceiveData();

   Sleep(2000); // While showing window (not window in mean) with players name in frontend TODO 2000->const

 
   game->SetGameStage(GAME_STAGE::running);
   while (game->getGameStage() == GAME_STAGE::running || game->getGameStage() == GAME_STAGE::result) {
      Sleep(1000);
      Json::Value jsonFromServer;

      game->GetGameFrameJSON(jsonFromServer);

      // backend_connector.SendData(jsonFromServer);
      ai_connector[0].SendData(jsonFromServer);
      ai_connector[1].SendData(jsonFromServer);

      Json::Value jsonFromAi1 = ai_connector[0].ReceiveData();
      std::cout << "Received from AI1" << std::endl << Json::StyledWriter().write(jsonFromAi1) << std::endl;
      game->SetJsonFromAi(jsonFromAi1, 0);

      Json::Value jsonFromAi2 = ai_connector[1].ReceiveData();
      std::cout << "Received from AI2" << std::endl << Json::StyledWriter().write(jsonFromAi2) << std::endl;
      game->SetJsonFromAi(jsonFromAi2, 1);

      game->RenderNextFrame();

      {
         std::ofstream ofstr(path + std::string("objects.json"));

         Json::StyledStreamWriter wrt;

         wrt.write(ofstr, jsonFromServer);
      }
   }
   Sleep(2000); // While showing results
}
