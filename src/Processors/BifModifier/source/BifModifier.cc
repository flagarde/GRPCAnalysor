#include "BifModifier.h"
#include "Global.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <cstdint>
#include <algorithm>
#include "Processor.h"
#include "IMPL/LCTOOLS.h"
#include "UTIL/CellIDEncoder.h"
#include "UTIL/CellIDDecoder.h"
#include "IMPL/LCCollectionVec.h"
#include <iomanip>
#include "Utilities.h"
#include "IMPL/LCRunHeaderImpl.h"
#include <IMPL/LCParametersImpl.h>
#include <fstream>
#include "EVENT/LCRunHeader.h" 
#include <algorithm> 
#include "EVENT/CalorimeterHit.h" 
#include "IMPL/ClusterImpl.h"
#include "Types.h"
TH1F diff("diff","diff",1000,-500,500);

void BifModifier::CalculateEfficiencyScinti(std::vector<RawCalorimeterHit *>vec)
{
  unsigned roll=0;
  for(std::vector<CalorimeterHit *>::iterator it=vec.begin(); it!=vec.end(); ++it) 
  {
    TOTALNUMBERHITSCINTI++;
	  roll++;
	  if(roll==1)EffiwithDiscri[geom.GetDifNbrPlate(dif_id)-1]+=1;
  }
}

void BifModifier::RealignBIF()
{
  if(_borneinf>_bornesup)
  {
    int three=_borneinf;
    _borneinf=_bornesup;
    _bornesup=three;
  }
  for(std::map< int,CalorimeterHit*>::iterator it=BIFetc.begin();it!=BIFetc.end();++it)
  {
    if(_borneinf==0&&_bornesup==0&&(it->second)->getEnergy()==3)BIFSelected.push_back(it->first);
    else
    {
      bool selected=false;
      std::map< int,CalorimeterHit*>::iterator itone=it;
      std::map< int,CalorimeterHit*>::iterator ittwo=it;
      for(unsigned int i=0;i!=abs(_borneinf+2);++i)
      {
        if(_borneinf<0)if(itone!=BIFetc.begin())--itone;
        if(_borneinf>0)if(itone!=BIFetc.end())++itone;
      }
      for(unsigned int i=0;i!=abs(_bornesup+2);++i)
      {
        if(_bornesup<0)if(ittwo!=BIFetc.begin())--ittwo;
        if(_bornesup>0)if(ittwo!=BIFetc.end())++ittwo;
      }
      for(std::map< int,CalorimeterHit*>::iterator itt=itone;itt!=ittwo;++itt)
      {
        if((it->first-itt->first)>=_borneinf&&(it->first-itt->first)<=_bornesup&&(_borneinf!=0&&_bornesup!=0))
        {
          if(((it->second)->getEnergy()==2)&&((itt->second)->getEnergy()==1))selected=true;
          if(_borneinf<0&&_bornesup>0&&(it->second)->getEnergy()==3)selected=true;
        }
      }
      if(selected==true)BIFSelected.push_back(it->first);
    }
  }
}

BifModifier aBifModifier;

BifModifier::BifModifier() : Processor("BifModifier")
{
  _outFileName="";
  registerProcessorParameter("LCIOOutputFile","LCIO file",_outFileName,_outFileName);
  _hcalCollections={"XYZFilled"};
  registerInputCollections( LCIO::CALORIMETERHIT,"HCALCollections","HCAL Collection Names",_hcalCollections,_hcalCollections);
  _borneinf=0;
  registerProcessorParameter("BifBornInf" ,"BifBornInf",_borneinf,_borneinf); 
  _bornesup=0;
  registerProcessorParameter("BifBornInf" ,"BifBornInf",_bornesup,_bornesup); 
} // end constructor

BifModifier::~BifModifier() {}

void BifModifier::processRunHeader( LCRunHeader* run){}

void BifModifier::init()
{ 
  printParameters();
}

void BifModifier::processEvent( LCEvent * evtP )
{
  for(unsigned int i=0; i< _hcalCollections.size(); i++) 
  {
    try 
	  {
	    LCCollection* col = evtP ->getCollection(_hcalCollections[i].c_str());
      CellIDDecoder<CalorimeterHit>decode(col);
      for (int ihit=0; ihit < col->getNumberOfElements();; ++ihit) 
      {
        CalorimeterHit* raw_hit = dynamic_cast<CalorimeterHit*>( col->getElementAt(ihit)) ;
        if (raw_hit != nullptr) 
	      {
	        if(Global::geom->GetDifType(decode(raw_hit)["DIF_Id"])==scintillator||Global::geom->GetDifType(decode(raw_hit)["DIF_Id"])==tcherenkov)
	        {
	          BIFetc[raw_hit->getTime()]=raw_hit;
	        }
        } 
	    } 
      RealignBIF();
      for(std::map< int,int>::iterator itt=Times.begin(); itt!=Times.end(); ++itt) 
	    {
	      bool Scintillatorseeittoo=false;
	      for(std::vector<int>::iterator iu=BIFSelected.begin();iu!=BIFSelected.end();)
	      {
	        diff.Fill(*iu-itt->first);
	        if(*iu-itt->first>=-6&&*iu-itt->first<=-3) 
	        {
	          Scintillatorseeittoo=true;
	          iu=BIFSelected.erase(iu);
	        }
	        else ++iu;
	      }
	      std::map<int,std::vector<CalorimeterHit *> >::iterator middle=RawHits.find(itt->first);
	      std::map<int,std::vector<CalorimeterHit *> >::iterator after=middle;
	      std::map<int,std::vector<CalorimeterHit *> >::iterator before=middle;
	      while(fabs(middle->first-before->first)<=_timeWin && before!=RawHits.begin()) --before;
	      ++before;
	      while(fabs(after->first-middle->first)<=_timeWin && after!=RawHits.end()) ++after;
	      std::map<int,int> nbrPlanestouched;
	      nbrPlanestouched.clear();
	      for(middle=before; middle!=after; ++middle ) 
	      {  
	        for(int unsigned i=0; i<(middle->second).size(); ++i) 
		      {
		        nbrPlanestouched[Global::geom->GetDifNbrPlate(decode((middle->second)[i])["DIF_Id"])]++;
		      }
	      }
	      std::vector<CalorimeterHit*> EventsGroupedScin;
	      for(middle=before; middle!=after;++middle ) 
		    {
		      EventsGroupedScin.insert(EventsGroupedScin.end(),middle->second.begin(),middle->second.end());
		    }
	      CalculateEfficiencyScinti(EventsGroupedScin);
	      if(nbrPlanestouched.size()>=(unsigned int)(_LayerCut)) 
	      {
	        LCEventImpl*  evtt = new LCEventImpl() ;
	        LCCollectionVec* clu3 = new LCCollectionVec(LCIO::CALORIMETERHIT);
          CellIDEncoder<CalorimeterHitImpl> cdt3( "I:8,J:7,K:10,DIF_Id:8,Asic_Id:6,Channel:7" ,clu3) ;
	        clu3->setFlag(clu3->getFlag()|( 1 << LCIO::RCHBIT_LONG));
	        clu3->setFlag(clu3->getFlag()|( 1 << LCIO::RCHBIT_TIME));
	        EventsSelected++;
	        for(middle=before; middle!=after; ) 
		      {
		        for(std::vector<CalorimeterHit *>::iterator itt=middle->second.begin();itt!=middle->second.end();++itt)
		        {
		          clu3->addElement(*itt);
		        }
		        if(std::find(BIFSelected.begin(),BIFSelected.end(),middle->first)!=BIFSelected.end())
		        {
		          BIFSelected.erase(std::find(BIFSelected.begin(),BIFSelected.end(),middle->first));
		        }
		        RawHits.erase(middle++);
		        
		      }
		      
          LCCollectionVec* clu4 = new LCCollectionVec(LCIO::CALORIMETERHIT);
          CellIDEncoder<CalorimeterHitImpl> cdt4( "I:8,J:7,K:10,DIF_Id:8,Asic_Id:6,Channel:7" ,clu4) ;
          clu4->setFlag(clu4->getFlag()|( 1 << LCIO::RCHBIT_LONG));
          clu4->setFlag(clu4->getFlag()|( 1 << LCIO::RCHBIT_TIME));
	        for(std::map<int,std::vector<CalorimeterHit *> >::iterator it=RawHits.begin();it!=RawHits.end();++it)
	        {
		        for(std::vector<CalorimeterHit *>::iterator itt=it->second.begin();itt!=it->second.end();++itt)
            {
              clu4->addElement(*itt);
            }

	        }
	        if(Scintillatorseeittoo)
	        {
		        evtt->addCollection(clu4,"REJECTED+BIF");
		        evtt->addCollection(clu3,"TRIVENTED+BIF");
	        }
	        else
	        {
	          evtt->addCollection(clu4,"REJECTED");
		        evtt->addCollection(clu3,"TRIVENTED");
	        }
          delete evtt;
          delete clu3;
          delete clu4;
          evtt->setEventNumber(EventsSelected);
	        evtt->setTimeStamp(evtP->getTimeStamp()*1e9);
	        evtt->setRunNumber(evtP->getRunNumber());
          _EventWriter->writeEvent( evtt ) ;
	      }
	    } 
	    BIFetc.clear();
	    BIFSelected.clear();
	  }
    catch (DataNotAvailableException &e)
	  {
	    std::cout << "collection skipped ..."<<std::endl;
	    colskipped++;
	    break;
	  }
  }
}


void BifModifier::end()
{  
  std::cout << "BifModifier::end() !! "<<colskipped<<"collection(s) skipped"<< std::endl;
}
