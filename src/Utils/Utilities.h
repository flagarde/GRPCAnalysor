#ifndef Utilities
#define Utilities
#include <iostream> 
#include "Colors.h"
#include <map>
#include <string>
#include <cstdlib>

const std::string Shift(double val)
{
  std::stringstream ss;
	if(val<10) ss<<"  "<<val;
	if(val>=10&&val<1000) ss<<" "<<val;
	if(val>=1000) ss<<val;
  return ss.str();
}
#endif
