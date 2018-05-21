#pragma once
#include <vector>
#include "Processor.h"
#include <array>
#include <set>
#include "Global.h"
#include <string>
#include "Types.h"
#include "TH1F.h"
class BifHelper : public Processor
{
  public:
    BifHelper();
    ~BifHelper();
    Processor* newProcessor()
    {
        return new BifHelper();
    }
    void init();
    void processEvent(EVENT::LCEvent *evtP);
    void processRunHeader( LCRunHeader* run);
    void end();
  private:  
    bool isusefull;
    std::vector<std::string> _hcalCollections;
    std::multiset<double>RawHits;
    std::array<std::multiset<double>,3>BIFetc;
    std::array<TH1F*,3> DistrDifTimeBIF;
    std::array<TH1F*,3> DistrDifTimeBifAndDifs;
};
