#include<cstring>
#include"helpingClass.h"
#include<fstream>
#include"state.h"

  State::State(const std::string& name1,bool final1,bool start1):stateName(name1),final(final1),start(start1){

  }
  //initializer list

  State::State()
  {
    stateName=" ";
    start=0;
    final=0;
  }
    //default constructor

  State::State(const State& other)
  {
    stateName=other.stateName;

    final=other.final;

    start=other.start;
  }

    //copy constructor

  void State::setStateName(const std::string &name)
  {
    if(!Helper::IsValidName(name))
    {
      throw "Invalid state name!";
    }
    stateName=name;
  }

 const std::string &State::getStateName() const
  {
    return stateName;
  }
  //getter and a setter for the name

  bool State::isFinal() const
  {
    return final;
  }
  bool State:: isStart()const
  {
    return start;
  }
  void State:: setFinal(bool const value)
  {
    final=value;
  }
  void State:: setStart(bool const value)
  {
    start=value;
  }
  State& State:: operator=(const State&other)
  {
     if (this==&other)
     {
        return *this;
     }
     final=other.final;
     start=other.start;
     stateName=other.stateName;
     return *this;
     
  }
 std::ostream&  operator<<(std::ostream& out,const State&other)
 {
    out<<other.stateName<<" ["<<other.final<<","<<other.start<<"]";//changed the brackets , so it wont confuse with the text files later
    return out;
 }

 State::State( State &&RhS)
  {
    stateName=std::move(RhS.stateName);
    final=RhS.final;
    start=RhS.start;
  }
  State& State::operator=( State&&RhS)
  {
    if(this==&RhS)
    {
      return *this;
    }
    stateName=std::move(RhS.stateName);
    final=RhS.final;
    start=RhS.start;
    return *this;

  }
  std::istream& operator>>(std::istream& in, State& state)
{
   std::string stateName;
   std::string stateValues;
   in>>stateName;
   in>>stateValues;

   state.stateName=stateName;
   
   state.final=stateValues[1]-'0';
   state.start=stateValues[3]-'0';
   return in;
}

bool State::operator==(const State &other) const
  {
      return stateName==other.stateName && start==other.start && final==other.final;
  }