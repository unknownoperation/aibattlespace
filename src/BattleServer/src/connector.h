#pragma once

#include <Json/json-forwards.h>

class CONNECTOR {
public:
   void SendGameFrameJSON (const Json::Value & scene);

   bool SetConnection     (void);
   void ReleaseConnection (void);

private:
   // infinity connection support
};
