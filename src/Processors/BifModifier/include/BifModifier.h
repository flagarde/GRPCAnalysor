#ifndef BifModifier
#define BifModifier
#include <map>
#include <vector>
#include "Processor.h"
#include "EVENT/RawCalorimeterHit.h"
#include "IMPL/LCCollectionVec.h"
#include "UTIL/CellIDEncoder.h"
#include "IMPL/CalorimeterHitImpl.h"
#include "EVENT/LCRunHeader.h"
#include "IMPL/LCEventImpl.h"
#include <set>
#include <string>
#include <iomanip>
#include <fstream>
#include <iostream>

class BifModifier : public Processor
{
public:
    BifModifier();
    ~BifModifier();
    Processor *newProcessor()
    {
        return new BifModifier();
    }
    void init();
    void processEvent(EVENT::LCEvent *evtP);
    void processRunHeader( LCRunHeader* run);
    void end();
    void RealignBIF();
    void CalculateEfficiencyScinti(RawCalorimeterHit* hit);
    private:  
    std::vector<std::string> _hcalCollections;
    std::map<int,int>Times;
    unsigned int EventsSelected;
    unsigned int eventtotal;
    int colskipped;
    int _borneinf;
    int _bornesup;
    LCWriter* _EventWriter;
    std::map< int,CalorimeterHit*> BIFetc;
    std::vector<int> BIFSelected;
    std::vector<int>EffiwithDiscri;
};
#endif
