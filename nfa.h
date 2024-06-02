#pragma once
#include"State.h"
#include"deltaTransition.h"
#include<vector>
#include<string>

class NFA{
    public:
    NFA(unsigned _id,const std::vector<DeltaTransition*>& _transitions);

    NFA(const NFA&other);

    NFA()=default;

    NFA& operator=(const NFA&other);

    NFA(NFA &&other);

    NFA& operator=(NFA &&other);

    ~NFA();

    const std::vector<State*> getStates();
    
    unsigned getId()const;

    void Print() const;

    void Save(std::string filename)const;

    bool Empty()const;

    bool Finite();

    bool Determinisation()const;//need researching

    bool Recognize(std::string word);//asked for a little help from the assisent of this subject

    NFA Union(const NFA &other)const;//ready 

    NFA Concat(const NFA &other)const;//ready

    NFA linearHull()const;//got it from a classmate(this is L+)

    NFA KleeneStar()const;//its just linearHull()+epsilone

    NFA getUniqueStates(const NFA&other)const;//a support function

    

      const std::vector<State*>connectedState(const State& start,const std::vector<State*>&visited)const;

    void setId(unsigned);//going to need it for the interface class in the future

    const std::vector<State*>getFinalStates()const;//also const for the same reason the Start states are

    std::vector<State*>TraversalWithChar(const std::vector<State*>&, const char c); //help function for the Recognize()

    const std::vector<State*>getStartStates()const;//needs to be const so Determinisation() can work properly

    bool FindPaths(const State& start, std::vector<State*>& visitedStates)const;

    bool static ContainsState(const State& state,const std::vector<State*>&vec);

    friend std::istream& operator>>(std::istream& in, NFA& automata);

    friend std::ostream& operator<<(std::ostream& out,const NFA& automata);

    static NFA nfaByWord(const std::string &word);

    static NFA nfaByRegex(const std::string &regex);

    private:
    unsigned int id; //unique idetification number

    std::vector<State*>states;//vector of the states in the NFA

    std::vector<DeltaTransition*>transitions;//veector of transitions

    static bool correctWord(const std::string& word);

    void updateStates();//getting in the array
    
    bool haveState(const std::string)const;//if it contains the name

    bool succCycle(const State& start,std::vector<State*> visitedStates,bool hasCycle)const;

    bool CycleFromState(const State& state,const std::vector<State*>&visitedStates)const;

    bool transitionIsVisited(const DeltaTransition*,std::vector<DeltaTransition*>&visitedEdges)const;

    void myDestruct();
    
    void copy(const NFA &other);
   
};