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
      compliting,
      unknown
   };
   const std::vector<std::string> stages{
      "connecting",
      "starting",
      "running",
      "result",
      "compliting"
   };
   GAME_STAGE ParseGameStage(std::string gameStage);

   class PNT {
   public:
      PNT() : x(0), y(0) {}
      PNT(int newX, int newY) : x(newX), y(newY) {}

      bool operator== (const PNT & point) const
      {
         return x == point.x && y == point.y;
      }

      int x, y;
   };

   enum class CELL_TYPE {
      space = 0,
      barrier
   };

   class FIELD;

class UNIT : public PNT {
public:
   UNIT(PNT point, int id);
   void Move(DIRECTION dir, FIELD gameMap);
private:
   int id;
};



