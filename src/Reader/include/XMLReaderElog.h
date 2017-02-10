#ifndef XMLREADERELOG_H
#define XMLREADERELOG_H
#include "Reader.h"
#include <iostream>
#include <string>
#include "tinyxml.h"
#include "ConfigInfo.h"
class XMLReaderElog: public Reader
{
  public:
  void Read(std::string &FileName,ConfigInfos* config){};
  void Read(std::string &FileName,ConfigInfos* config,int RunNumber);
  void Read(std::string& FileName,Geometry& geom){};
  Reader* Clone() { return new XMLReaderElog(); }
  ~XMLReaderElog(){};
};
#endif
