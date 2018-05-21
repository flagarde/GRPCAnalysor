#pragma once
#include "Processor.h"
#include <vector>
#include <map>
#include <string>
#include "UTIL/CellIDDecoder.h"
#include "UTIL/CellIDEncoder.h"
#include "IMPL/CalorimeterHitImpl.h"
#include "ConstructConverters.h"

using namespace lcio ;

class XYZfiller : public Processor
{
  public:
    virtual Processor*  newProcessor()
    {
      return new XYZfiller ;
    }
    XYZfiller() ;
    ~XYZfiller(){};
    virtual void init() ;
    virtual void processRunHeader( LCRunHeader* run ) ;
    virtual void processEvent( LCEvent * evt ) ;
    virtual void end() ;
    void FillXYZ(CalorimeterHitImpl*, LCCollectionVec*,CellIDEncoder<CalorimeterHitImpl>&,CellIDDecoder<CalorimeterHitImpl>&);
    std::vector<std::string> _hcalCollections;
    std::map<int,bool>Warningg;
    ConstructConverters* converter;
    bool _SupressHitsOfDifsNotInXML;
};
