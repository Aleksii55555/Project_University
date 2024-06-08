#pragma once
#include<fstream>
#include<string>
#include<vector>
#include"nfa.h"

class Interface{

  public:
  void Run();

  static Interface& Init();//initialize

  Interface(const Interface&)=delete;

  Interface(const Interface&&)=delete;

  Interface& operator=(const Interface&)=delete;

  Interface& operator=(const Interface&&)=delete;
  private:
  static bool fileIsOpened;

  static std::string fileMessage;

  static Interface* interface;

  std::vector<NFA*>nfas;

  std::string openedfile;

  Interface()=default;

  ~Interface();
  
  void myDestructor();
  
  void Open(const std::string&);

  void Save(const std::string&filename)const;

  void printNfa(unsigned const id)const;

  void Union(unsigned const id1,unsigned const id2);

  void Concat(unsigned const id1,unsigned const id2);
  
  void linearHull(unsigned const id);

  void Close();

  void Help()const;

  void Exit();

  void Empty(unsigned const id)const;

  void Determinisation(unsigned id)const;

  void nfaByRegex(const std::string &regex);
  
  public:

  void List()const;

  void readingTxt(std::istream&);

  static const std::string GetComand(const std::string);


};