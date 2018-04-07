#pragma once

#include <string>
#include "AILib.h"

class PLAYER : public PLAYER_BASE {
public:
   PLAYER(const std::string& url);
   void run_game();
};
