#pragma once
#include <iostream>

using namespace std;

class DBField{

 private :
  string name;
  string type;
  int length;

 public :
  DBField(string n, string t, int l);
  string getName();
  string getType();
  int getLength();

};
