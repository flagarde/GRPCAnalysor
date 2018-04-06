#ifndef AEGIS_h
#define AEGIS_h
#include "Processor.h"
#include <string>
#include "UTIL/CellIDDecoder.h"
#include "UTIL/CellIDEncoder.h"
#include "IMPL/CalorimeterHitImpl.h"
#include <fstream>
using namespace lcio ;

class AEGIS : public Processor
{
  public:
    virtual Processor*  newProcessor()
    {
      return new AEGIS ;
    }
    AEGIS() ;
    ~AEGIS(){};
    virtual void init() ;
    virtual void processRunHeader( LCRunHeader* run ) ;
    virtual void processEvent( LCEvent * evt ) ;
    virtual void end() ;
    std::vector<std::string> _hcalCollections;
    std::ofstream TXTFile;
};
#endif
