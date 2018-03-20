#pragma once

#include <string>

#include <Json/json-forwards.h>

class CONNECTOR {
public:
   void SendGameFrameJSON (const Json::Value & scene);

   bool SetConnection     (const std::string& url);
   void ReleaseConnection (void);
private:
   // infinity connection support
	int socket;

	void error(const std::string& func_name);
};
