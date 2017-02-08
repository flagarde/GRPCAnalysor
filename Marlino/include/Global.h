#ifndef Global_h
#define Global_h 1
#include "OutFileRoot.h"
#include "Colors.h"
#include "Geometry.h"
#include "ConfigInfo.h"
#include "StringParameters.h"
class Global
{
  public:
  static StringParameters* parameters ;
  static OutFileRoot* out;
  static Geometry* geom;
  static int number;
  static int totalevent;
  static ConfigInfos* conf;
};
#endif
