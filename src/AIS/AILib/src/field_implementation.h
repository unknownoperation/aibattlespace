#pragma once

#include <vector>
#include "field_base.h"

class FIELD_IMPLEMENTATION : public FIELD_BASE {
public:
   double time;

   enum class CELL_TYPE {
      space = 0,
      barrier,
      chips
   };
   std::vector<std::vector<CELL_TYPE>> field;

   class UNIT {
   public:
      int id;
      int x, y;
   };
   std::vector<UNIT> units;

   void ParseinitialData(const Json::Value& data) override;
   void ParseData(const Json::Value& data) override;
   Json::Value GetData() override;
private:
   std::vector<std::vector<CELL_TYPE>> basic_field;
};