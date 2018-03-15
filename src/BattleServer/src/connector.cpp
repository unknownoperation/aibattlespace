#include <string>

#include <json/json.h>

#include "connector.h"

void CONNECTOR::SendGameFrameJSON (const Json::Value & scene)
{
   Json::StyledWriter writer;


   std::string str = writer.write(scene);

   // send string
}

bool CONNECTOR::SetConnection (void)
{
   return false;
}

void CONNECTOR::ReleaseConnection (void)
{
}
