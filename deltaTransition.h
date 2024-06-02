#pragma once
#include"State.h"
#include<vector>
#include<fstream>


class DeltaTransition
{
    private:
    State* start;
    State* end;
    char label;

   public:
   DeltaTransition(const State &start1,const State &end1, char label1);
   //initializer list

   DeltaTransition();
   //default constructor

   DeltaTransition(const DeltaTransition&other);
   //copy constructor

   DeltaTransition& operator=(const DeltaTransition &other);

   DeltaTransition(DeltaTransition &&other);

   DeltaTransition& operator=(DeltaTransition &&other);

   ~DeltaTransition();
   //destructor


   char getLabel()const;

   State* getStart()const;

   State* getEnd()const;
   //getters, because the variables are private

   friend std::ostream& operator<<(std::ostream& out, const DeltaTransition &relation);

   friend std::istream& operator>>(std::istream& in, DeltaTransition& relation);
   //operators


};