#include <Windows.h>
#include "AILib.h"
#include "field_pick.h"
#include "game_common.h"


///construcor
///used establish connection to server


struct PIMPL_PLAYER_BASE {
   std::string playerName;
   std::string serverAdress;
   FIELD_BASE *field;
   GAME_STAGE *stage;
};



PLAYER_BASE::PLAYER_BASE()
{
   pimpl->serverAdress = "tcp://127.0.0.1:8800";
   int port = std::stoi(pimpl->serverAdress.substr(pimpl->serverAdress.size() - 4, 4));
   std::string adress = pimpl->serverAdress;
   adress = (std::string)adress.substr(0, adress.size() - 4);
   while (!TWO_WAY_CONNECTOR::SetConnection(adress + std::to_string(port), TWO_WAY_CONNECTOR::CONNECTION_TYPE::bind)) {
      ++port;
   }
   TWO_WAY_CONNECTOR::SendData("Initial message");
   Json::Value data  = TWO_WAY_CONNECTOR::ReceiveData();
   pimpl->playerName = data["key"].asString();
   pimpl->field = new FIELD_MANAGER();
   pimpl->field->ParseinitialData(data);
   pimpl->stage = new GAME_STAGE();
}

PLAYER_BASE::~PLAYER_BASE()
{
   TWO_WAY_CONNECTOR::ReleaseConnection();
   delete pimpl->field;
   delete pimpl->stage;
}

FIELD_BASE * PLAYER_BASE::GetField()
{
   return pimpl->field;
}
///getting and parsing data which are delievered from server
void PLAYER_BASE::GetData()
{
   Json::Value msg = TWO_WAY_CONNECTOR::ReceiveData();
   *(pimpl->stage) = ParseGameStage(msg["game_stage"].asString());
   pimpl->field->ParseData(msg);
}
///sending data to server
void PLAYER_BASE::SendData(std::vector<UNIT_RESPONSE> data)
{
   Json::Value msg;

   for (int i = 0; i < data.size(); ++i) {
      msg["players"][i]["unitID"] = data[i].id;
      msg["players"][i]["direction"] = GetDirStr(DIRECTION((int)data[i].direction));
   }
   msg["key"] = pimpl->playerName;
   TWO_WAY_CONNECTOR::SendData(msg);
}
///getting current stage of the game
GAME_STAGE PLAYER_BASE::GetGameStage()
{
   return *(pimpl->stage);
}
///getting ID of AI
int PLAYER_BASE::GetMyPlayerID()
{
   return pimpl->playerName[0] - '0';
}

