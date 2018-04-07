#pragma once

#include <string>
#include <json/json.h>

class TWO_WAY_CONNECTOR {
public:
   enum class CONNECTION_TYPE {
      bind = 0,
      connect
   };
   bool SetConnection(const std::string& url, CONNECTION_TYPE connection_type = CONNECTION_TYPE::connect);
   void ReleaseConnection(void);
   int SendData(const Json::Value& data);
   Json::Value ReceiveData(void);
private:
   int socket;
   void error(const std::string& func_name);
};