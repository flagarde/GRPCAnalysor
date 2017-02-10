#ifndef Progress_h
#define Progress_h
#include "Processor.h"
#include <string>
using namespace lcio ;

class Progress : public Processor
{
  public:
    virtual Processor*  newProcessor()
    {
      return new Progress ;
    }
    Progress() ;
    ~Progress(){};
    virtual void init() ;
    virtual void processRunHeader( LCRunHeader* run ) ;
    virtual void processEvent( LCEvent * evt ) ;
    virtual void end() ;
    //inline std::string Shift(double);
    inline unsigned int Every(unsigned int &);
    inline void Progression(unsigned int,unsigned int, unsigned int, unsigned int,std::string Perso);
};
#endif
