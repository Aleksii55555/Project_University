#include"nfa.h"
#include<iostream>
#include"helpingClass.h"
#include <fstream>
#include<exception>
NFA::NFA(unsigned _id,const std::vector<DeltaTransition*>& _transitions){
  id=_id;
   for (int i = 0; i < _transitions.size(); i++)
  {
    transitions.push_back(new DeltaTransition(*_transitions[i]));//copying transitions properly now
  }
  updateStates();

};


NFA::NFA(const NFA &other)
{
    id=other.id;
    copy(other);
    updateStates();
}

NFA& NFA::operator=(const NFA &other)
{
    if(this==&other)
    {
        return *this;
    }
    myDestruct();
    copy(other);
    updateStates();
    return *this;
}
NFA::~NFA(){
myDestruct();
}
NFA::NFA(NFA &&other)
{
   id=other.id;
   transitions=std::move(other.transitions);

}
NFA& NFA::operator=(NFA &&other)
{
    if (this == &other)
    {
        return *this;
    }
    id = other.id;

    transitions=std::move(other.transitions);
    return *this;
}
unsigned NFA::getId() const
{
    return id;
}
const std::vector<State*> NFA:: getStates()
{
   updateStates();
   return states;
}
void NFA::Print() const
{
    for (DeltaTransition* delta : transitions)
    {
        std::cout<<*delta<<std::endl;   
    }
}

 NFA NFA:: getUniqueStates(const NFA&other)const
 {  
    std::vector<DeltaTransition*>newTransitions=std::vector<DeltaTransition*>(transitions.size()+other.transitions.size());
    for (int i = 0; i < transitions.size(); i++)
    {
       newTransitions[i]=new DeltaTransition (*transitions[i]);
    }
    for (int i = 0; i < other.transitions.size(); i++)
    {
        std::string newStart=other.transitions[i]->getStart()->getStateName();

        std::string newEnd=other.transitions[i]->getEnd()->getStateName();

        while (other.haveState(newStart) || this->haveState(newStart))
        {
            newStart=newStart+"00";
        }
        while (other.haveState(newEnd) || this->haveState(newEnd))
        {
            newEnd=newEnd+"00";
        }
        State resultStartState= State(*other.transitions[i]->getStart());
        State resultEndState=State(*other.transitions[i]->getEnd());

        resultStartState.setStateName(newStart);
        resultEndState.setStateName(newEnd);

        newTransitions[transitions.size()+i]=new DeltaTransition(resultStartState,resultEndState,other.transitions[i]->getLabel());        
    }
    //with this method we assure that the states name wont be the same, when we work with two NFAs
   
    NFA result;
    result.id=0;
    result.transitions=std::move(newTransitions);
    result.updateStates();
    return result;
 }
void NFA::updateStates()
 {

     for (DeltaTransition* delta : transitions)
     {
         if (haveState(delta->getStart()->getStateName())==false)
         {
             states.push_back(delta->getStart());
         }
         if (haveState(delta->getEnd()->getStateName())==false)
         {
             states.push_back(delta->getEnd());
         }
     }
 };

bool NFA:: haveState(const std::string name)const
{    

    for(State* state:states)
    {
        if(state->getStateName()==name)
        {
            return true;
        }
    }
    return false;
}
//support function for update states

 NFA NFA:: Union(const NFA &other)const
 {
     NFA empty_nfa;
     bool empt;
     if (this->Empty() || other.Empty())
     {

         empt = true;
     }
     if (empt)
     {
         empty_nfa.getUniqueStates(other);
     }

    NFA union_nfa=this->getUniqueStates(other);
    return union_nfa;
 }
 //union is having the 2 nfas as one,assuring we have different state names

 NFA NFA::Concat(const NFA &other)const{
    NFA concat_nfa=this->getUniqueStates(other);

    for (int i = 0; i < states.size(); i++)
    {
        if (concat_nfa.states[i]->isFinal())
        {
            concat_nfa.states[i]->setFinal(false);
            for (int j = 0; j < other.states.size(); j++)
            {
                if (other.states[j]->isStart())
                {
                    concat_nfa.states[states.size()+j]->setStart(false);
                    concat_nfa.transitions.push_back(new DeltaTransition(*concat_nfa.states[i],*concat_nfa.states[states.size()+j],'~'));
                }                
            }           
        }
        
    }
    concat_nfa.updateStates();
    return concat_nfa;   
 }
 NFA NFA:: linearHull()const//this is the L+
 {
    NFA un_nfa = NFA(*this);
     for (size_t i = 0; i < states.size(); i++)
     {
         if (un_nfa.states[i]->isFinal())
         {
             for (size_t j = 0; j < states.size(); j++)
             {
                 if (un_nfa.states[j]->isStart())
                 {
                     un_nfa.transitions.push_back(new DeltaTransition(*un_nfa.states[i], *un_nfa.states[j], '~'));
                 }
             }
         }
     }
     un_nfa.updateStates();
     return un_nfa;
 }
 NFA NFA::KleeneStar() const
 {
    NFA star_nfa(this->linearHull());
    std::string epsiloneStateName="e";//We need a state that recognizes empty word
    while (star_nfa.haveState(epsiloneStateName))
    {
        epsiloneStateName+='e';
    }
    State epsiloneState = State(epsiloneStateName,1,1);
    star_nfa.transitions.push_back(new DeltaTransition(epsiloneState,epsiloneState,'~'));
    star_nfa.updateStates();
    return star_nfa;
 }


 const std::vector<State*>NFA::getStartStates()const
 {
    std::vector<State*>start_states;
    for (State* state:states)
    {
        if (state->isStart())
        {
            start_states.push_back(state);
        }

    }
    return start_states;

    std::cout<<"End of the start states ";

 }
 //helping function that adds the start States in a vector
 const std::vector<State*>NFA::getFinalStates()const
 {
    std::vector<State*>final_states;
    
    for (State* state:states)
    {
        if (state->isFinal())
        {
            final_states.push_back(state);
        }

    }
    return final_states;

 }//helping function that adds the final States in a vector

 bool NFA:: Empty()const
 {
if(getFinalStates().empty() || getStartStates().empty())
   {
      return true;
   }
   std::vector<State*>visited;
   const std::vector<State*>startStates=getStartStates();
   for (State* state:startStates)
   {
    visited.push_back(state);
     if (FindPaths(*state,visited))
     {
        return false;
     }
    visited.clear();
   } 
   return true;
 }


 std::vector<State*> NFA::TraversalWithChar(const std::vector<State*>&current_states, const char c)
 {
   std::vector<State*>result;
   for (int i = 0; i < current_states.size(); i++)
   {
     for (int j = 0; j < transitions.size(); j++)
     {
        if (*current_states[i]==*transitions[j]->getStart()&&transitions[j]->getLabel()==c)
        {
            result.push_back(transitions[j]->getEnd());
        }       
     }

   }
   return result;
 }

  bool NFA:: Recognize(std::string word)//got help from the assistent of this subject
  {
    std::vector<State*>current;
  
    Helper::moveElemToDest(current,TraversalWithChar(getStartStates(),word[0]));
    for (int i = 1; i < word.size(); i++)
    {
        if(current.empty())
        {
            return false;
        }

         Helper::moveElemToDest(current,TraversalWithChar(current,word[i]));
        
    }
    for (State* state:current)
    {
        if(state->isFinal())
        {
            return true;
        }
    } 
    return false;
  }
  void NFA:: Save(std::string filename)const
  {
    std::ofstream os;

    os.open(filename,std::ios::app);

    

    os<<'\n'<<transitions.size()<<'\n';//first we print a new line,than the number of transitions

     for (int i=0;i<transitions.size();++i)
    {
        os<<*transitions[i]<<'\n';
    }
    os<<*transitions[transitions.size()-1];
   os.close();
  }

  std::istream& operator>>(std::istream& input, NFA& automata)
  {
    int relationsCount;

    input>>relationsCount;
    for (int i = 0; i <relationsCount; i++)
    {
        DeltaTransition* delta=new DeltaTransition();

        input>>*delta;

        automata.transitions.push_back(delta);
    }
    automata.updateStates();
    return input;
  }

  std::ostream &operator<<(std::ostream &out, const NFA &automata)
  {
     out<<'\n'<<automata.transitions.size()<<'\n';
    for (int i=0;i<automata.transitions.size()-1;++i)
    {
        out<<*automata.transitions[i]<<'\n';
    }
    out<<*automata.transitions[automata.transitions.size()-1];
     
     return out;
  }
  void NFA::setId(unsigned id1)
 {
    id=id1;
 }
 bool NFA::Determinisation()const
{    bool deter=false;
     if (getStartStates().size() > 1)
      {
          return false;
      }
      else{

        deter=true;
      }
      const int alphabet_size = 36;

      int states_count = 1;

      states_count = states.size();

      bool alphabet[states_count][alphabet_size] = {
          0,
      };
      for (int i = 0; i < states_count; ++i)
      {
          for (DeltaTransition *delta : transitions)//checking for espilones
          {
              if (delta->getLabel() == '~') 
              {
                  return false;
              }

              if (*states[i] == *delta->getStart())
              {

                  if (delta->getLabel() >= 48 && delta->getLabel() <= 57) //checking for numbers
                  {
                      if (alphabet[i][delta->getLabel() - 48])
                      {
                          return false;
                      }
                      alphabet[i][delta->getLabel() - 48] = true;
                  }
                  if (delta->getLabel() >= 97 && delta->getLabel() <= 122) //checking for letters
                  {
                      if (alphabet[i][delta->getLabel() - 87])
                      {
                          return false;
                      }
                      alphabet[i][delta->getLabel() - 87] = true;
                  }
              }
          }
      }
      return true;
}
bool NFA::ContainsState(const State &state, const std::vector<State *> &states1)
 {
    for (State* s:states1)
    {
        if (state==*s)
        {
            return true;
        }
    }

     return false;
 }
 
 const std::vector<State *> NFA::connectedState(const State &start,const std::vector<State*>&visited) const
 {
    std::vector<State*> new_states;
    std::vector<State*> old_states;
    for (DeltaTransition* delta:transitions)
    {
        if (start==*delta->getStart() || ContainsState(*delta->getEnd(),visited))
        {
            old_states.push_back(delta->getStart());
        }

    }



    for (DeltaTransition* delta:transitions)
    {
        if (start==*delta->getStart() &&!ContainsState(*delta->getEnd(),visited))
        {
            new_states.push_back(delta->getEnd());
        }

    }


     return new_states;
 }
bool NFA::FindPaths(const State &start,  std::vector<State *> &visitedStates) const
 {  
    if(start.isFinal())
    {
      return true;
    }
    if (connectedState(start,visitedStates).empty()&&!start.isFinal())
    {
      return false;
    }
    for (DeltaTransition* delta:transitions)
    {
        if (start==*delta->getStart()&&!ContainsState(*delta->getEnd(),visitedStates))
        {
            visitedStates.push_back(delta->getEnd());
            return FindPaths(*delta->getEnd(),visitedStates);
        }
        
    }
  return false;
 }

 

  void NFA::myDestruct()
 {
    for (DeltaTransition* delta:transitions)
    {
        delete delta;
    }
    
 }
 bool NFA::succCycle(const State &start, std::vector<State *> visitedStates,bool hasCycle) const
 {
   // if (CycleFromState(start,visitedStates))
    // {
      
    //   hasCycle=1;

    // }
    if (connectedState(start,visitedStates).empty())
    {
        std::cout<<"Return here";
        return hasCycle;
    }
    
    for (DeltaTransition* delta:transitions)
    {
        if (start==*delta->getStart()&&!ContainsState(*delta->getEnd(),visitedStates))
        {
            for (State* state:visitedStates)
            {
                std::cout<<"visited: "<<state->getStateName()<<'\n';
            }
            
            std::cout<<"state: "<<start.getStateName()<<"\n";
            visitedStates.push_back(delta->getEnd());
           
            
            if(CycleFromState(*delta->getEnd(),visitedStates))
            {
                if(delta->getEnd()->isFinal())
                {
                    hasCycle=true;
                    return true;
                }
                
                hasCycle=true;
            }
            return succCycle(*delta->getEnd(),visitedStates,hasCycle);
        }
        
    }
     return false;
 }
  bool NFA::Finite()
 {
   if(getFinalStates().empty()|| getStartStates().empty())
   {
      return true;
   }
   std::vector<State*>visitedStates;

   std::vector<DeltaTransition*>visitedTransitions;

   const std::vector<State*>startStates=getStartStates();

   const std::vector<State*>endStates=getFinalStates();
   bool hasCycle;

   for (State* state:startStates)
   {
      hasCycle=0;
      visitedStates.push_back(state);
      if(succCycle(*state,visitedStates,hasCycle))//cycle path=not finite language
      {
        return false;
      }
      visitedStates.clear();
      visitedTransitions.clear();
   }
   
   return true;
 }
 bool NFA:: CycleFromState(const State& state,const std::vector<State*>&visitedStates)const
{
    for (DeltaTransition*delta:transitions)
    {
       if (state==*delta->getStart() && ContainsState(*delta->getEnd(),visitedStates));
       {
         return true;
       }
    }
    return false;

}
bool NFA::transitionIsVisited(const DeltaTransition * delta, std::vector<DeltaTransition *> &visitedEdges) const
 {
    for (DeltaTransition* deltaRel:visitedEdges)
    {
        if (delta==deltaRel)
        {
            return true;
        }   
    }
     return false;
 }
 bool NFA::correctWord(const std::string& word)//just checking if the word is in the alphabet
 {
    for (size_t i = 0; i < word.size(); i++)
    {
       if((word[i]<'a'||word[i]>'z')&&(word[i]<'0'||word[i]>'9'))
       {
          return false;
       }
    }

    return true;
 }
 NFA NFA::nfaByWord(const std::string &word)
 {
    if(!correctWord(word))
    {
        throw "Word is not from the alphabet(a-z && 0-9) !";
    }
    NFA automata;
    State start("s",0,1);
    for (int i = 0; i < word.size()-1; i++)
    {
      State end(std::to_string(i),0,0);
      DeltaTransition delta(start,end,word[i]);
      automata.transitions.push_back(new DeltaTransition(start,end,word[i]));
      start=end;
    }
    State final("f",1,0);
    automata.transitions.push_back(new DeltaTransition(start,final,word[word.size()-1]));

    automata.updateStates();
     return automata;
 }
 NFA NFA::nfaByRegex(const std::string& regex)//count the brackets with recursion 
 {
    
    std::string regxWithoutBrackets=regex.substr(1,regex.size()-2);

    unsigned bracketsCount=0;

    int strLen=regxWithoutBrackets.size();


    if (correctWord(regxWithoutBrackets))
    {
        return NFA::nfaByWord(regxWithoutBrackets);
    }
    for (int i = 0; i < regxWithoutBrackets.size(); i++)
    {
        if(regxWithoutBrackets[i]=='('){
         bracketsCount++;
         }
        else if(regxWithoutBrackets[i]==')'){
         bracketsCount--;
         }
        if (bracketsCount==0)
        {
            if(regxWithoutBrackets[i]=='*')
            {
                return NFA::nfaByRegex(regxWithoutBrackets.substr(0,i)).KleeneStar();
            }

            else if(regxWithoutBrackets[i]=='+')
            {
              return NFA::nfaByRegex(regxWithoutBrackets.substr(0,i)).Union(NFA::nfaByRegex(regxWithoutBrackets.substr(i+1,strLen-i-1)));
            }

            else if(regxWithoutBrackets[i]=='.')
            {
              return NFA::nfaByRegex(regxWithoutBrackets.substr(0,i)).Concat(NFA::nfaByRegex(regxWithoutBrackets.substr(i+1,strLen-i-1)));  
            }
        }

    }



     return NFA();
 }

 void NFA::copy(const NFA &other)
 {
    id=other.id;
    for (size_t i = 0; i < other.transitions.size(); i++)
    {
      transitions.push_back(new DeltaTransition(*other.transitions[i]));
    }

 }

