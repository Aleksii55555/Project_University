#include"Interface.h"
#include<iostream>
#include<vector>
#include<fstream>
#include<exception>
#include"helpingClass.h"
#include<string>

Interface* Interface::interface=nullptr;

bool Interface::fileIsOpened=0;

std::string Interface::fileMessage="There is not an opened file!\n";

Interface::~Interface()
{
  myDestructor();
}

Interface& Interface:: Init()
{
    
  if(interface!=nullptr)
  {
    return *interface;
  }
  interface= new Interface();
  return *interface;   
  
}
enum CommandType {
    CMD_EXIT,
    CMD_CLOSE,
    CMD_HELP,
    CMD_OPEN,
    CMD_RECOGNIZE,
    CMD_DETERMINISTIC,
    CMD_EMPTY,
    CMD_SAVE,
    CMD_SAVEAS,
    CMD_LIST,
    CMD_PRINT,
    CMD_UNION,
    CMD_CONCAT,
    CMD_LINEAR_HULL,
    CMD_REGEX,
    CMD_INVALID
};
CommandType GetCommandType(const std::string& command) {
    if (command == "exit") return CMD_EXIT;
    if (command == "close") return CMD_CLOSE;
    if (command == "help") return CMD_HELP;
    if (command.substr(0, 4) == "open") return CMD_OPEN;
    if (command == "recognize") return CMD_RECOGNIZE;
    if (command.substr(0, 13) == "deterministic") return CMD_DETERMINISTIC;
    if (command.substr(0, 5) == "empty") return CMD_EMPTY;
    if (command.substr(0, 4) == "save") return CMD_SAVE;
    if (command.substr(0, 6) == "saveas") return CMD_SAVEAS;
    if (command == "list") return CMD_LIST;
    if (command.substr(0, 5) == "print") return CMD_PRINT;
    if (command.substr(0, 5) == "union") return CMD_UNION;
    if (command.substr(0, 6) == "concat") return CMD_CONCAT;
    if (command.substr(0, 2) == "L+") return CMD_LINEAR_HULL;
    if (command.substr(0, 3) == "reg") return CMD_REGEX;
    return CMD_INVALID;
}


void Interface::Run() {
    std::cout << "Type help to view the possible commands!\n";
    std::string command;
    std::getline(std::cin, command);

    while (true) {
        CommandType commandType = GetCommandType(command);

        switch (commandType) {
            case CMD_EXIT:
                Exit();
                break;

            case CMD_CLOSE:
                if (fileIsOpened) {
                    Close();
                } else {
                    std::cout << fileMessage;
                }
                break;

            case CMD_HELP:
                Help();
                break;

            case CMD_OPEN:
                if (!fileIsOpened) {
                    std::string filename = command.substr(5);
                    Open(filename);
                } else {
                    std::cout << "This file is already opened\n";
                }
                break;

            case CMD_RECOGNIZE:
                if (!fileIsOpened) {
                    std::cout << fileMessage;
                } else {
                    // Recognize logic here
                }
                break;

            case CMD_DETERMINISTIC:
                if (!fileIsOpened) {
                    std::cout << fileMessage;
                } else {
                    try {
                        size_t id = std::stoi(command.substr(14));
                        Determinisation(id);
                    } catch (const std::exception &e) {
                        std::cout << "Something went wrong!\n";
                    }
                }
                break;

            case CMD_EMPTY:
                if (!fileIsOpened) {
                    std::cout << fileMessage;
                } else {
                    try {
                        size_t id = std::stoi(command.substr(6));
                        Empty(id);
                    } catch (const std::exception &e) {
                        std::cout << "Something went wrong!\n";
                    }
                }
                break;

            case CMD_SAVE:
                if (!fileIsOpened) {
                    std::cout << fileMessage;
                } else if (command.size() != 4) {
                    try {
                        size_t id = std::stoi(command.substr(5));
                        if (id < 0 || id >= nfas.size()) {
                            std::cout << "A NFA with this ID doesn't exist! Type list to see all available NFAs!\n";
                        } else {
                            std::string fileName = command.substr(6 + std::to_string(id).length());
                            nfas[id]->Save(fileName);
                            std::cout << "You have successfully saved automata with id " << id << " into " << fileName << '\n';
                        }
                    } catch (const std::exception &e) {
                        std::cout << "Something went wrong!\n";
                    }
                } else {
                    try {
                        Save(openedfile);
                    } catch (const char* err) {
                        std::cout << err;
                    } catch (const std::exception &e) {
                        std::cout << "Something went wrong!\n";
                    }
                }
                break;

            case CMD_SAVEAS:
                try {
                    std::string fileName = command.substr(7);
                    Save(fileName);
                } catch (const std::exception &e) {
                    std::cout << "Something went wrong!\n";
                }
                break;

            case CMD_LIST:
                if (!fileIsOpened) {
                    std::cout << fileMessage;
                } else {
                    List();
                }
                break;

            case CMD_PRINT:
                if (!fileIsOpened) {
                    std::cout << fileMessage;
                } else {
                    try {
                        unsigned automataId = std::stoi(command.substr(6));
                        printNfa(automataId);
                    } catch (const std::exception &e) {
                        std::cout << "Something went wrong!\n";
                    }
                }
                break;

            case CMD_UNION:
                if (!fileIsOpened) {
                    std::cout << fileMessage;
                } else {
                    try {
                        size_t spacePos = command.find(' ', 6);
                        unsigned id1 = std::stoi(command.substr(6, spacePos - 6));
                        unsigned id2 = std::stoi(command.substr(spacePos + 1));
                        Union(id1, id2);
                    } catch (const std::exception &e) {
                        std::cout << "Something went wrong!\n";
                    }
                }
                break;

            case CMD_CONCAT:
                if (!fileIsOpened) {
                    std::cout << fileMessage;
                } else {
                    try {
                        size_t spacePos = command.find(' ', 7);
                        unsigned id1 = std::stoi(command.substr(7, spacePos - 7));
                        unsigned id2 = std::stoi(command.substr(spacePos + 1));
                        Concat(id1, id2);
                    } catch (const std::exception &e) {
                        std::cout << "Something went wrong!\n";
                    }
                }
                break;

            case CMD_LINEAR_HULL:
                if (!fileIsOpened) {
                    std::cout << fileMessage;
                } else {
                    try {
                        unsigned id = std::stoi(command.substr(3));
                        linearHull(id);
                    } catch (const std::exception &e) {
                        std::cout << "Something went wrong!\n";
                    }
                }
                break;

            case CMD_REGEX:
                if (!fileIsOpened) {
                    std::cout << fileMessage;
                } else {
                    std::string regex = command.substr(4);
                    nfaByRegex(regex);
                }
                break;

            default:
                std::cout << "Invalid command\n";
                break;
        }

        command.clear();
        std::getline(std::cin, command);
    }
}

void Interface::Close()
{
    
     myDestructor();

     nfas.clear();

     fileIsOpened=0;

     std::cout<<openedfile<<" is closed!\n";

     openedfile="";
    

}

void Interface::Exit()
{
    std::cout<<"Exiting the program!";
    myDestructor();
    exit(0);
}//basic built-in function 

void Interface::Open(const std::string &filename) 
{
    std::ifstream in;
        in.open(filename);
        if(!in.good())
        {
          std::ofstream out(filename);
          if(out.is_open())
          {
            out.close();
          }
          else 
          {
           throw "coudn't open the file";
          } 
          in.open(filename);
        }
        if (in.is_open())
        {
          readingTxt(in);
          openedfile = filename;
          fileIsOpened = true;
          in.close();
          std::cout<<"Successfully opened "<<filename<<'\n';
        }
        else throw "coudn't open the file";
      
}
void Interface::Help() const
{
  std::cout<<"You can choose from the following commands:\n"<<

  "open <file_name> -> opens that file\n"<<

  "close -> closes the opened file\n"<<

  "save -> saves the opened file\n"<<

  "save <id> <file_name> -> saves the automata with that id in that file with <file_name>!\n"<<

  "saveas <file_name> -> saves the currently opened file in <file_name>\n"<<

  "help -> prints this info\n"<<

  "list -> prints the id-s of all read automatas\n"<<

  "exit -> exists the program\n"<<

  "union <id1> <id2> ->creates the union between id1 and id2 with new id\n"<<

  "concat <id1> <id2> ->creates the concat between id1 and id2 with new id\n"<<

  "empty <id> -> Prints weather the nfa is empty or not\n"<<

  "deterministic <id> -> Prints weather the nfa is empty or not\n"<<

  "finite <id> -> Prints weather the nfa is empty or not\n";

  

 
 


}

void Interface::List() const
{
   std::cout<<"There are "<<nfas.size()<<" read automata/s with id from 0 to "<<nfas.size()-1<<'\n';
}

void Interface::readingTxt(std::istream& in){
  
in.ignore();
  while (!in.eof())
   {
     NFA* a=new NFA();
     in>>*a;
     a->setId(nfas.size());
     nfas.push_back(a);
   }
}

const std::string Interface::GetComand(const std::string input)
{
    std::string command;

    for (char space:input)
    {
      if(space==' ')
      {
        return command;
      }
      command+=space;
    }
    return command;
}
void Interface::myDestructor()
{
  for (NFA *a : nfas)
  {
    delete a;
  }
}
void Interface::Save(const std::string& fileName) const
{
  std:: ofstream out;
  out.open(fileName,std::ios::trunc);
  if(out.is_open())
  {
    for (NFA*a:nfas)
    {
      out<<*a;
    }
    out.close();
    std::cout<<"Successfully saved "<<fileName<<'\n';
  }
  else throw "Coud't save the file!";
}
void Interface::printNfa(unsigned const id) const
{
  if(id<0 || id>=nfas.size())
  {
    std::cout<<"A NFA with this ID doesnt exist!\n";
    return;
  }
  nfas[id]->Print();
}
void Interface::Union(unsigned const id1, unsigned const id2)
{
  if(id1<0 ||id1>nfas.size())

  {
    std::cout<<"A NFA with this ID(the first) doesnt exist!";
    return;
  }

  if(id2<0||id2>nfas.size())
  {
    std::cout<<"A NFA with this ID(the second) doesnt exist!";
    return;
  }
  NFA* union_nfa=new NFA();
  *union_nfa=nfas[id1]->Union(*nfas[id2]);

  union_nfa->setId(nfas.size());
  nfas.push_back(union_nfa);

  std::cout<<"Succesfully created a union of "<<id1<<" and "<<id2<<"with id: "<<union_nfa->getId()<<'\n';


}
void Interface::Concat(unsigned const id1, unsigned const id2)
{
  if(id1<0 || id1>nfas.size())
  {
    std::cout<<"A NFA with this ID doesnt exist!\n";
    return;
  }
  if(id2<0 || id2>nfas.size())
  {
    std::cout<<"A NFA with this ID doesnt exist!\n";
    return;
  }
  NFA* concat_nfa=new NFA();
  *concat_nfa=nfas[id1]->Concat(*nfas[id2]);
  concat_nfa->setId(nfas.size());
  nfas.push_back(concat_nfa);
  std::cout<<"Succesfully created a concat of "<<id1<<" and "<<id2<<"with id: "<<concat_nfa->getId()<<'\n';
}

void Interface::linearHull(unsigned const id)
{
  if(id<0 || id>=nfas.size())
  {
    std::cout<<"A NFA with this ID doesnt exist!\n";
    return;
  }
  NFA* linearHull_nfa=new NFA();
  *linearHull_nfa=nfas[id]->linearHull();
  
  linearHull_nfa->setId(nfas.size());
  nfas.push_back(linearHull_nfa);

  std::cout<<"Created a L+ of "<<id<<" with id: "<<linearHull_nfa->getId()<<'\n';
}
void Interface::Empty(const unsigned id) const {
    if (id >= nfas.size()) {
        std::cout << "A NFA with this ID doesn't exist!\n";
        return;
    }
    std::cout << std::boolalpha << nfas[id]->Empty() << '\n';
}
void Interface::Determinisation(unsigned id) const
{
  if (id < 0 || id >= nfas.size())
  {
    std::cout << "A NFA with this ID doesnt exist!\n";
    return;
  }

  std::cout << std::boolalpha << nfas[id]->Determinisation() << '\n';
}
void Interface::nfaByRegex(const std::string &regex)
{
    NFA* nfa= new NFA();
  
    *nfa=NFA::nfaByRegex(regex);
    
    nfa->setId(nfas.size());

    nfas.push_back(nfa);

    std::cout<<"Succesfully created automata by regular expression with id: "<<nfa->getId()<<'\n';

    delete nfa;
   
//реминдер
}