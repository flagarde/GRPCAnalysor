#ifndef Clusterisation_h
#define Clusterisation_h
#include "Processor.h"
#include "Clustering.h"
#include "RawHit_standard_merge_predicate.h"
#include <vector>
#include <string>
#include "HistoPlane.h"
using namespace lcio ;

class Clusterisation : public Processor
{
  public:
    virtual Processor*  newProcessor()
    {
      return new Clusterisation ;
    }
    Clusterisation() ;
    ~Clusterisation(){};
    virtual void init() ;
    virtual void processRunHeader( LCRunHeader* run ) ;
    virtual void processEvent( LCEvent * evt ) ;
    virtual void end() ;
    std::vector<std::string> _hcalCollections;
    std::vector<CalorimeterHit*> HitsByPlanes;
    bool poundGain;
    bool poundThreshold;
    int m_neighbourTimeDistance;
    int m_neighbourStripDistance;
    int m_neighbourPadDistance;
    bool m_padJunctionByCorner;
    HistoPlane hists;
};
#endif
