#pragma once

#include <vector>
#include <string>


   enum class DIRECTION {
      up = 0,
      right,
      down,
      left,
      none
   };

   DIRECTION GetDirEnum(const std::string & str);

   std::string GetDirStr(const DIRECTION & dir);


   enum class GAME_STAGE {
      connecting = 0,
      starting,
      running,
      result,
      compliting
   };
   const std::vector<std::string> stages{
      "connecting",
      "starting",
      "running",
      "result",
      "compliting"
   };

    class PNT {
    public:
       PNT() : x(0), y(0) {}
       PNT(int newX, int newY) : x(newX), y(newY) {}
       int x, y;
    };

    enum class CELL_TYPE {
       space = 0,
       barrier,
       chip
    };

    class FIELD;

class UNIT : public PNT {
public:
   UNIT(PNT point, int id);
   void Move(DIRECTION dir, FIELD gameMap);
private:
   int id;
};



