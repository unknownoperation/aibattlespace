#include <Windows.h>
#include "AILib.h"
#include "field_pick.h"
#include "game_common.h"

PLAYER_BASE::PLAYER_BASE(const std::string & serverAdress) :
   serverAdress(serverAdress)
{
   int port = std::stoi(serverAdress.substr(serverAdress.size() - 4, 4));
   std::string adress = serverAdress;
   adress = (std::string)adress.substr(0, adress.size() - 4);
   while (!TWO_WAY_CONNECTOR::SetConnection(adress + std::to_string(port))) {
      ++port;
   }
   TWO_WAY_CONNECTOR::SendData("Initial message");
   Json::Value data  = TWO_WAY_CONNECTOR::ReceiveData();
   playerName = data["key"].asString();
   field = new FIELD_MANAGER();
   field->ParseinitialData(data);
}

PLAYER_BASE::~PLAYER_BASE()
{
   TWO_WAY_CONNECTOR::ReleaseConnection();
   delete field;
}

FIELD_BASE * PLAYER_BASE::GetField()
{
   return field;
}

void PLAYER_BASE::GetData()
{
   field->ParseData(TWO_WAY_CONNECTOR::ReceiveData());
}

void PLAYER_BASE::SendData(std::vector<UNIT_RESPONSE> data)
{
   Json::Value msg;

   for (int i = 0; i < data.size(); ++i) {
      msg["players"][i]["unitID"] = data[i].id;
      msg["players"][i]["direction"] = GetDirStr(DIRECTION((int)data[i].direction));
   }
   msg["key"] = playerName;
   TWO_WAY_CONNECTOR::SendData(msg);
}

GAME_STAGE PLAYER_BASE::GetGameStage()
{
   return stage;
}
