#include "TriventTriggered.h"
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

TriventTriggered aTriventTriggered;

TriventTriggered::TriventTriggered() : Processor("TriventTriggered")
{
  _outFileName="LCIO_clean_run.slcio";
  registerProcessorParameter("LCIOOutputFile","LCIO file",_outFileName,_outFileName);
  _hcalCollections={"XYZFilled"};
  registerInputCollections( LCIO::CALORIMETERHIT,"HCALCollections","HCAL Collection Names",_hcalCollections,_hcalCollections);
  _noiseCut = 7;
  registerProcessorParameter("NoiseCut" ,"noise cut in time spectrum 7 in default",_noiseCut ,_noiseCut);
  _TriggerTime=0;
  registerProcessorParameter("TriggerTime" ,"All Events with Time greater than this number will be ignored (0) in case of Triggerless",_TriggerTime ,_TriggerTime);
} // end constructor

TriventTriggered::~TriventTriggered() {}

void TriventTriggered::processRunHeader( LCRunHeader* run){}

void TriventTriggered::init()
{ 
  _EventWriter = LCFactory::getInstance()->createLCWriter() ;
  _EventWriter->setCompressionLevel( 2 ) ;
  _EventWriter->open(_outFileName.c_str(),LCIO::WRITE_NEW) ;
  printParameters();
}

std::string name1="blabla";
void TriventTriggered::processEvent( LCEvent * evtP )
{
  for(unsigned int i=0; i< _hcalCollections.size(); i++) 
  {
    try 
	  {
	    static int event=0;
	    event++;
	    LCCollection* col = evtP ->getCollection(_hcalCollections[i].c_str());
      int numElements = col->getNumberOfElements();
      CellIDDecoder<CalorimeterHit>decode(col);
      //////////test if timestamp negatif
      bool HasTimeStampNegative=false;
      for (int ihit=0; ihit < numElements; ++ihit) 
      {
        CalorimeterHit* raw_hit = dynamic_cast<CalorimeterHit*>( col->getElementAt(ihit)) ;
        if (raw_hit != nullptr) 
	      {
	        if(Global::geom->GetDifType(decode(raw_hit)["DIF_Id"])==scintillator||Global::geom->GetDifType(decode(raw_hit)["DIF_Id"])==tcherenkov)continue;
	        if(Global::geom->GetDifNbrPlate(decode(raw_hit)["DIF_Id"])==-1) continue;
	        if(raw_hit->getTime()<0)
	        {
	          //std::vector<unsigned int>b{dif_id,(unsigned int)((raw_hit->getCellID0() & 0xFF00)>>8),(unsigned int)((raw_hit->getCellID0() & 0x3F0000)>>16)}; Negative[b][raw_hit->getTimeStamp()]++;
	          //std::cout<<"TimeStamp <=-1 : "<<raw_hit->getTimeStamp()<<std::endl;
	          if(raw_hit->getTime()<-1)HasTimeStampNegative=true;         
	        }
	      }
      }
      LCEventImpl*  evtt = new LCEventImpl() ;
      LCCollectionVec* clu1 = new LCCollectionVec(LCIO::CALORIMETERHIT);
      CellIDEncoder<CalorimeterHitImpl> cdt( "I:8,J:7,K:10,Dif_id:8,Asic_id:6,Chan_id:7" ,clu1) ;
	    clu1->setFlag(clu1->getFlag()|( 1 << LCIO::RCHBIT_LONG));
	    clu1->setFlag(clu1->getFlag()|( 1 << LCIO::RCHBIT_TIME));
	    bool selected=false;
      for (int ihit=0; ihit < numElements; ++ihit) 
      {
        CalorimeterHit* raw_hit = dynamic_cast<CalorimeterHit*>( col->getElementAt(ihit)) ;
        if (raw_hit != nullptr) 
	      {
	        if(Global::geom->GetDifType(decode(raw_hit)["DIF_Id"])==scintillator||Global::geom->GetDifType(decode(raw_hit)["DIF_Id"])==tcherenkov)continue;
	        if(Global::geom->GetDifNbrPlate(decode(raw_hit)["DIF_Id"])==-1)
	        {
	          if(Warningg[decode(raw_hit)["DIF_Id"]]!=true) 
		        {
		          Warningg[decode(raw_hit)["DIF_Id"]]=true;
		          std::cout<<"Please add DIF "<<decode(raw_hit)["DIF_Id"]<<" to your geometry file; I'm Skipping its data."<<std::endl;
		        }
	          continue;
	        }
	        if(_TriggerTime==0 || raw_hit->getTime()<=_TriggerTime)
	        {
	          if(HasTimeStampNegative==false)
	          {
	            RawHits[decode(raw_hit)["DIF_Id"]].push_back(raw_hit);
		        }
		      }
        } 
	    }
	    for(std::map< int,std::vector<CalorimeterHit*> >::iterator it=RawHits.begin();it!=RawHits.end();++it)
	    {
	      if(it->second.size()>=_noiseCut)
	      {
	        selected=false;
	        break;
	      }
	      else selected=true;
	    }
	    if(selected==true)
	    {
	      EventsSelected++; 
	      for(std::map< int,std::vector<CalorimeterHit*> >::iterator it=RawHits.begin();it!=RawHits.end();++it)
	      {
	        for(unsigned int o=0;o!=it->second.size();++o)
	        {
	          clu1->addElement(it->second[o]);
	        }
	      }
	      evtt->addCollection(clu1,"Triggered");
        evtt->setEventNumber(EventsSelected);
	      evtt->setTimeStamp(evtP->getTimeStamp()*1e9);
	      evtt->setRunNumber(evtP->getRunNumber());
        _EventWriter->writeEvent( evtt ) ;
      }
      RawHits.clear();
	  }
    catch (DataNotAvailableException &e)
	  {
	    std::cout << "TRIGGER SKIPED ..."<<std::endl;
	    _trig_count++;
	    break;
	  }
  }
}


void TriventTriggered::end()
{  
  _EventWriter->close();
  for(std::map<int,bool>::iterator it=Warningg.begin(); it!=Warningg.end(); it++) std::cout<<red<<"REMINDER::Data from Dif "<<it->first<<" are skipped !"<<normal<<std::endl;
  std::cout << "TriventProcess::end() !! "<<_trig_count<<" Events skiped"<< std::endl;
  std::cout <<TouchedEvents<<" Events were overlaping "<<"("<<(TouchedEvents*1.0/(TouchedEvents+eventtotal))*100<<"%)"<<std::endl;
  std::cout <<"Total nbr Events : "<<eventtotal<<" : "<<EventsSelected<<" ("<<EventsSelected*1.0/eventtotal*100<<"%)"<< std::endl;
  /*std::cout<<green<<"Total time of the File : "<<total_time_file<<"s Total time of selected events : "<<total_time_by_user<<"s ; Time global ( Usefull Time ) : "<<HistoPlanes[0]->Get_Global_Total_Time()*200e-9<<"s; In percent : "<<HistoPlanes[0]->Get_Global_Total_Time()*100.0*200e-9/total_time_by_user<<"% of usefull time and "<<total_time_by_user*100.0/total_time_file<<"% of time selected in file"<<normal<<std::endl;*/
}
