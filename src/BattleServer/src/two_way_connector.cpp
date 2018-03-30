#include <iostream>

#define NN_STATIC_LIB
#include <nn.h>
#include <pair.h>

#include "two_way_connector.h"

bool TWO_WAY_CONNECTOR::SetConnection(const std::string& url)
{
   if ((socket = nn_socket(AF_SP, NN_PAIR)) < 0) {
      error("nn_socket");
      return false;
   }
   if (nn_connect(socket, url.c_str()) < 0) {
      error("nn_connect");
      return false;
   }

   int to = 300000;
   if (nn_setsockopt(socket, NN_SOL_SOCKET, NN_RCVTIMEO, &to, sizeof(to)) < 0) {
      error("nn_setsockopt");
      ReleaseConnection();
      return false;
   }

   return true;
}

void TWO_WAY_CONNECTOR::ReleaseConnection(void)
{
   nn_shutdown(socket, 0);
}

int TWO_WAY_CONNECTOR::SendData(const Json::Value& data)
{
   std::string msg(Json::StyledWriter().write(data));

   return nn_send(socket, msg.c_str(), msg.length() + 1, 0);
}

Json::Value TWO_WAY_CONNECTOR::ReceiveData(void)
{
   char *msg = nullptr;
   std::string data;
   int result = nn_recv(socket, &msg, NN_MSG, 0);

   if (result > 0) {
      data = msg;
      nn_freemsg(msg);
   }

   return data;
}

void TWO_WAY_CONNECTOR::error(const std::string & func_name)
{
   std::cerr << func_name << ": " << nn_strerror(nn_errno()) << std::endl;
}
