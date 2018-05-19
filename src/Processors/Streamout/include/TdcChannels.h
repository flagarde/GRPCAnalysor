#pragma once
#include <iostream>
#include <iomanip>
#include <limits>
#include "Global.h"
#include <string.h>
#include <cmath>
class TdcChannel
{
public:
  TdcChannel(uint8_t*  b){ memcpy(_fr,b,8);}
  inline uint8_t channel() {return  (_fr[0]&0XFF);}
  inline void setIP(uint8_t IP){_IP=IP;}
  inline void setMezzanine(uint8_t mezzanine){_mezzanine=mezzanine;}
  inline int IP() {return _IP;}
  inline uint8_t length(){return 8;}
  inline uint64_t coarse(){return ((uint64_t)_fr[6])|((uint64_t)_fr[5]<<8)|((uint64_t)_fr[4]<<16)|((uint64_t)_fr[3]<<24);}
  inline uint8_t fine(){return _fr[7];}
  inline int mezzanine() {return _mezzanine;}
  #ifdef BCIDFROMCOARSE
  inline uint16_t bcid(){return (uint16_t) (coarse()*2.5/200);}
  #else
  inline uint16_t bcid(){return (_fr[2]|(((uint16_t)_fr[1])<<8));}
  #endif
  inline double tdcTime(){ return (coarse()*2.5+fine()*0.009765625);}
  inline uint8_t* frame(){ return _fr;}
private:
  uint8_t _fr[8];
  int _mezzanine{-1};
  int _IP{-1};
};

/*static std::ostream& operator<<(std::ostream& flux, TdcChannel& c)
{
  flux << " Channel " << std::setw(4) << (int) c.channel()
       << ", BCID "  << std::setw(8) << c.bcid()
       << ", coarse Value " << std::setw(12) << c.coarse()
       << ", fine value " << std::setw(5) << (int) c.fine()
       << ", TDCtime " << std::setprecision (std::numeric_limits<double>::digits10+1) << c.tdcTime();
  return flux;
}*/
