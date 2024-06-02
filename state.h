#pragma once
#include <string>
#include <ostream>

class State
{
    private:
    std::string stateName;
    bool start = 0;
    bool final = 0;
public:
    State(const std::string &_name, bool _final, bool _start);

    State();

    State(const State &other);

    State &operator=(const State &other);
    
    ~State() = default;

    const std::string& getStateName() const;

    State(State &&RhS);

    void setStateName(const std::string &name);

    State& operator=( State&&RhS);

    //the big 6

    bool isFinal() const;

    bool isStart() const;

    bool operator==(const State &other)const;

    void setFinal(bool const value);

    void setStart(bool const value);
    
    friend std::ostream& operator<<(std::ostream& out,const State&other);

    friend std::istream &operator>>(std::istream &in,  State &other);

};