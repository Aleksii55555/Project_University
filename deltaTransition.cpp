#include "deltaTransition.h"
#include<fstream>

DeltaTransition:: DeltaTransition(const State &start1, const State &end1, char label1){
  start=new State(start1);

  end=new State(end1);

  label=label1;
}//using the new move function so it is easy the memory sharing

DeltaTransition::DeltaTransition()
{
    start=nullptr;
    end=nullptr;
    label=0;
}
DeltaTransition::DeltaTransition(const DeltaTransition&other)
{
   start=new State( *other.start);
    end=new State( *other.end);
   label=other.label;
}
DeltaTransition::~DeltaTransition()
{
    if(start!=nullptr)
    {
        delete start;
    }
    if(end!=nullptr)
    {
        delete end;
    }
}
DeltaTransition& DeltaTransition:: operator=(const DeltaTransition &other)
{
    if (this==&other)
    {
        return *this;
    }
    start=new State( *other.start);
    end=new State( *other.end);
    label=other.label;
    return *this;
    

}
char DeltaTransition:: getLabel()const
{
    return label;
}
State* DeltaTransition:: getStart()const
{
    return start;
}
State* DeltaTransition:: getEnd()const
{ 
    return end;
}
std::ostream& operator<<(std::ostream& out, const DeltaTransition &delta){
    out<< *delta.getStart()<< " "<<delta.getLabel()<<" "<< *delta.getEnd();
    return out;
}
std::istream& operator>>(std::istream& in, DeltaTransition& relation){
    if(relation.start!=nullptr)
    {
        delete []relation.start;
    }
    if(relation.end!=nullptr)
    {
        delete[]relation.end;
    }
    State* start=new State();

    State* end=new State();
    
    char c;
    in>>*start;
    in>>c;
    in>>*end;
    relation.start=start;
    relation.end=end;
    relation.label=c;
    return in;
}
DeltaTransition::DeltaTransition(DeltaTransition &&other){
    start=other.start;
    end=other.end;
    label=other.label;
    
    other.start=nullptr;
    other.end=nullptr;


}
DeltaTransition& DeltaTransition::operator=(DeltaTransition &&other)
{
    if(this==&other)
    {
        return *this;
    }
    delete start;
    delete end;

    start=other.start;
    end=other.end;
    label=other.label;

    other.start=nullptr;
    other.end=nullptr;

    return *this;
}
