#pragma once

#include <string>
#include <json/json.h>

class TWO_WAY_CONNECTOR {
public:
   bool SetConnection(const std::string& url);
   void ReleaseConnection(void);
   int SendData(const Json::Value& data);
   Json::Value ReceiveData(void);
private:
   int socket;
   void error(const std::string& func_name);
};