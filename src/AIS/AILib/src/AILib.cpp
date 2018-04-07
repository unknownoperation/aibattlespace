#include <Windows.h>
#include "AILib.h"
#include "field_implementation.h"

PLAYER_BASE::PLAYER_BASE(const std::string & serverAdress) :
   serverAdress(serverAdress)
{
   TWO_WAY_CONNECTOR::SetConnection(serverAdress);
   TWO_WAY_CONNECTOR::SendData("Initial message");
   Json::Value data  = TWO_WAY_CONNECTOR::ReceiveData();
   playerName = data["key"].asString();
   field = new FIELD_IMPLEMENTATION();
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
      msg["players"][i]["direction"] = directions[(int)data[i].direction];
   }
   msg["key"] = playerName;
   TWO_WAY_CONNECTOR::SendData(msg);
}

PLAYER_BASE::GAME_STAGE PLAYER_BASE::GetGameStage()
{
   return stage;
}
