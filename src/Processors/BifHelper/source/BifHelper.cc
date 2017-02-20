#include "BifHelper.h"
#include "Global.h"
#include <string>
#include "Processor.h"
#include "Types.h"
#include "UTIL/CellIDDecoder.h"
#include "EVENT/CalorimeterHit.h" 
BifHelper aBifHelper;

BifHelper::BifHelper() : Processor("BifHelper")
{
  _hcalCollections={"XYZFilled"};
  registerInputCollections( LCIO::CALORIMETERHIT,"HCALCollections","HCAL Collection Names",_hcalCollections,_hcalCollections);
}

BifHelper::~BifHelper() {}

void BifHelper::processRunHeader( LCRunHeader* run){}

void BifHelper::init()
{ 
  printParameters();
  isusefull=false;
  for(unsigned int i=0;i!=Global::geom->GetNumberPlates();++i)
  {
    if(Global::geom->GetDifType(i)==scintillator||Global::geom->GetDifType(i)==bif)
    {
      isusefull=true;
      for(unsigned int i=0;i!=BIFetc.size();++i)
      {
        std::string name1="Distribution of bif time signal threshold";
        for(unsigned int j=i+1;j!=BIFetc.size();++j)
        {
           name1+=std::to_string(j+1)+"-";
        }
        name1+=" bif time threshold"+std::to_string(i+1);
        std::string name2="distribution of hits times - bif time signal threshold"+std::to_string(i+1);
        DistrDifTimeBIF[i]=new TH1F(name1.c_str(),name1.c_str(),10000,-5000,5000);
        DistrDifTimeBifAndDifs[i]=new TH1F(name2.c_str(),name2.c_str(),10000,-5000,5000);
      } 
    }
  }
  if(isusefull==false)std::cout<<yellow<<"No Dif configured as bif or scintillator... This processor is useless "<<normal<<std::endl;
}

void BifHelper::processEvent( LCEvent * evtP )
{
  if(isusefull==false) return;
  for(unsigned int i=0; i< _hcalCollections.size(); i++) 
  {
    try 
	  {
	    LCCollection* col = evtP ->getCollection(_hcalCollections[i].c_str());
      CellIDDecoder<CalorimeterHit>decode(col);
      for (int ihit=0; ihit < col->getNumberOfElements(); ++ihit) 
      {
        CalorimeterHit* raw_hit = dynamic_cast<CalorimeterHit*>( col->getElementAt(ihit)) ;
        if (raw_hit != nullptr) 
	      {
	        if(Global::geom->GetDifType(decode(raw_hit)["DIF_Id"])==scintillator||Global::geom->GetDifType(decode(raw_hit)["DIF_Id"])==bif)
	        {
	          if(raw_hit->getEnergy()==2) BIFetc[0].insert(raw_hit->getEnergy());
	          else if (raw_hit->getEnergy()==1) BIFetc[1].insert(raw_hit->getEnergy());
	          else if (raw_hit->getEnergy()==3) BIFetc[2].insert(raw_hit->getEnergy());
	        }
		      RawHits.insert(raw_hit->getEnergy());
        } 
	    } 
      for(unsigned int i=0;i!=BIFetc.size();++i)
      {
        for(unsigned int j=i+1;j!=BIFetc.size();++j)
        {
          for(std::multiset<double>::iterator it=BIFetc[i].begin();it!=BIFetc[i].end();++it)
          {
            for(std::multiset<double>::iterator itt=BIFetc[j].begin();itt!=BIFetc[j].end();++itt)
            {
              DistrDifTimeBIF[i]->Fill(*it-*itt);
            }
          }
        }      
      }
      for(unsigned int i=0;i!=BIFetc.size();++i)
      {
        for(std::multiset<double>::iterator it=BIFetc[i].begin();it!=BIFetc[i].end();++it)
        {
          for(std::multiset<double>::iterator itt=RawHits.begin();itt!=RawHits.end();++itt)
          {
              DistrDifTimeBifAndDifs[i]->Fill(*itt-*it);
          }
        }
      }
      RawHits.clear();
      for(unsigned int u=0;u!=BIFetc.size();++u) BIFetc[u].clear();
	  }
    catch (DataNotAvailableException &e)
	  {
	    std::cout<<yellow<<"collection skipped ..."<<normal<<std::endl;
	    break;
	  }
  }
}

void BifHelper::end()
{
  if(isusefull==false) return;
  for(unsigned int i=0;i!=DistrDifTimeBIF.size();++i)
  {
    Global::out->writeObject("DistributionBifSignal",DistrDifTimeBIF[i]);  
    delete DistrDifTimeBIF[i];
  }
  for(unsigned int i=0;i!=DistrDifTimeBifAndDifs.size();++i)
  {
    Global::out->writeObject("DistributionBifHitsSignal",DistrDifTimeBifAndDifs[i]);  
    delete DistrDifTimeBifAndDifs[i];
  }
}
