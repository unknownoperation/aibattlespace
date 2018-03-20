#include <iostream>

#include <json/json.h>
#include <nn.h>
#include <pipeline.h>

#include "connector.h"

void CONNECTOR::SendGameFrameJSON (const Json::Value & scene)
{
   Json::StyledWriter writer;


   std::string str = writer.write(scene);

   // send string
   if (nn_send(socket, str.c_str(), str.size() + 1, 0) < 0) {
	   error("nn_send");
   }
}

bool CONNECTOR::SetConnection (const std::string& url)
{
	if ((socket = nn_socket(AF_SP, NN_PUSH)) < 0) {
		error("nn_socket");
		return false;
	}
	if (nn_connect(socket, url.c_str()) < 0) {
		error("nn_bind");
		return false;
	}
   return true;
}

void CONNECTOR::ReleaseConnection (void)
{
	nn_shutdown(socket, 0);
}

void CONNECTOR::error(const std::string & func_name)
{
	std::cerr << func_name << ": " << nn_strerror(nn_errno()) << std::endl;
}
