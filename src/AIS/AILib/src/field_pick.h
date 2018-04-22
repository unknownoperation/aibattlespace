#pragma once

#include <vector>

#include "json\json-forwards.h"

#include "game_common.h"
#include "field_base.h"
#include "field.h"


class FIELD_MANAGER : public FIELD_BASE {
public:
   void ParseinitialData(const Json::Value& data) override;
   void ParseData(const Json::Value& data) override;
   Json::Value GetData(void) override;

   std::vector<UNIT> players;
   FIELD field;
   std::vector<PNT> chips;
   double time;
};
