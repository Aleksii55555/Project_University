#pragma once
#include<vector>
#include "state.h"
#include<string>

class Helper
{
   public:
   
   static bool IsValidName(const std::string);

   static void moveElemToDest(std::vector<State*>&destination,  std::vector<State*>&&);
};