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
  _noiseCut = -1;
  registerProcessorParameter("NoiseCut" ,"Number of hits maximum per DIFs and events (-1) to ignore this parameter",_noiseCut ,_noiseCut);
  _TriggerTimeLow=-1;
  registerProcessorParameter("TriggerTimeLow" ,"All Events with Time lower than this number will be ignored (-1) in case of Triggerless",_TriggerTimeLow,_TriggerTimeLow);
  _TriggerTimeHigh=-1;
  registerProcessorParameter("TriggerTimeHigh" ,"All Events with Time higher than this number will be ignored (-1) in case of Triggerless",_TriggerTimeHigh,_TriggerTimeHigh);
  if(_TriggerTimeLow==-1)_TriggerTimeLow=0;
  if(_TriggerTimeHigh==-1)_TriggerTimeHigh=std::numeric_limits<int>::max();
  if(_noiseCut==-1)_noiseCut=std::numeric_limits<int>::max();
} // end constructor

TriventTriggered::~TriventTriggered() {}

void TriventTriggered::processRunHeader( LCRunHeader* run){}

void TriventTriggered::init()
{ 
  _EventWriter = LCFactory::getInstance()->createLCWriter() ;
  _EventWriter->setCompressionLevel( 2 ) ;
  _EventWriter->open(_outFileName.c_str(),LCIO::WRITE_NEW) ;
  printParameters();
  for(unsigned int i=0;i!=Global::geom->GetNumberPlates();++i)
  {
    if(Global::geom->GetDifType(i)!=temporal&&Global::geom->GetDifType(i)!=scintillator&&Global::geom->GetDifType(i)!=tcherenkov)
    {
      std::string name="Time Distribution in plate nbr "+std::to_string(i+1);
      std::string name2="Hits Distribution in plate nbr "+std::to_string(i+1);
      int Xmin=0;
      int Xmax=100000;
      int NBins=100000;
      if(_TriggerTimeLow!=0)Xmin=_TriggerTimeLow;
      if(_TriggerTimeHigh!=0)Xmax=_TriggerTimeHigh;
      NBins=Xmax-Xmin;
      TimeDistribution[i+1]=new TH1F(name.c_str(),name.c_str(),NBins,Xmin,Xmax);
      int xmax=Global::geom->GetSizeX(i)+1;
      int ymax=Global::geom->GetSizeY(i)+1;
      HitsDistribution[i+1]=new TH2F(name2.c_str(),name2.c_str(),xmax,0,xmax,ymax,0,ymax);
    }
  }
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
      LCEventImpl*  evtt = new LCEventImpl() ;
      LCCollectionVec* clu1 = new LCCollectionVec(LCIO::CALORIMETERHIT);
      LCCollectionVec* clu2 = new LCCollectionVec(LCIO::CALORIMETERHIT);
      CellIDEncoder<CalorimeterHitImpl> cdt( "I:8,J:7,K:10,Dif_id:8,Asic_id:6,Chan_id:7" ,clu1) ;
      CellIDEncoder<CalorimeterHitImpl> cdt2( "I:8,J:7,K:10,Dif_id:8,Asic_id:6,Chan_id:7" ,clu2) ;
	    clu1->setFlag(clu2->getFlag()|( 1 << LCIO::RCHBIT_LONG));
	    clu1->setFlag(clu2->getFlag()|( 1 << LCIO::RCHBIT_TIME));
	    clu2->setFlag(clu2->getFlag()|( 1 << LCIO::RCHBIT_LONG));
	    clu2->setFlag(clu2->getFlag()|( 1 << LCIO::RCHBIT_TIME));
	    bool selected=false;
      for (int ihit=0; ihit < numElements; ++ihit) 
      {
        CalorimeterHit* raw_hit = dynamic_cast<CalorimeterHit*>( col->getElementAt(ihit)) ;
        if (raw_hit != nullptr) 
	      {
	        if(Global::geom->GetDifType(decode(raw_hit)["DIF_Id"])==scintillator||Global::geom->GetDifType(decode(raw_hit)["DIF_Id"])==tcherenkov)continue;
	        if(Global::geom->GetDifNbrPlate(decode(raw_hit)["DIF_Id"])==-1)
	        {
	          continue;
	        }
	        if(_TriggerTimeLow<=raw_hit->getTime()&&raw_hit->getTime()<=_TriggerTimeHigh)
	        {
	          SelectedHits[decode(raw_hit)["DIF_Id"]].push_back(raw_hit);
	          if(Global::geom->GetDifNbrPlate(decode(raw_hit)["DIF_Id"])!=-1)
	          {
	            TimeDistribution[Global::geom->GetDifNbrPlate(decode(raw_hit)["DIF_Id"])]->Fill(raw_hit->getTime());
	            HitsDistribution[Global::geom->GetDifNbrPlate(decode(raw_hit)["DIF_Id"])]->Fill(decode(raw_hit)["I"],decode(raw_hit)["J"]);
	          }
		      }
		      else
		      {
		        RejectedHits[decode(raw_hit)["DIF_Id"]].push_back(raw_hit);
		      }
        } 
	    }
	    for(std::map< int,std::vector<CalorimeterHit*> >::iterator it=SelectedHits.begin();it!=SelectedHits.end();++it)
	    {
	      if(it->second.size()>=(unsigned int)(_noiseCut))
	      {
	        selected=false;
	        break;
	      }
	      else selected=true;
	    }
	    if(selected==true)
	    {
	      EventsSelected++; 
	      for(std::map< int,std::vector<CalorimeterHit*> >::iterator it=SelectedHits.begin();it!=SelectedHits.end();++it)
	      {
	        for(unsigned int o=0;o!=it->second.size();++o)
	        {
	          clu1->addElement(it->second[o]);
	        }
	      }
	      for(std::map< int,std::vector<CalorimeterHit*> >::iterator it=RejectedHits.begin();it!=RejectedHits.end();++it)
	      {
	        for(unsigned int o=0;o!=it->second.size();++o)
	        {
	          clu2->addElement(it->second[o]);
	        }
	      }
	      evtt->addCollection(clu1,"TriggeredSelected");
	      evtt->addCollection(clu2,"TriggeredRejected");
        evtt->setEventNumber(EventsSelected);
	      evtt->setTimeStamp(evtP->getTimeStamp()*1e9);
	      evtt->setRunNumber(evtP->getRunNumber());
        _EventWriter->writeEvent( evtt ) ;
      }
      SelectedHits.clear();
	  }
    catch (DataNotAvailableException &e)
	  {
	    std::cout << "TRIGGER SKIPED ..."<<std::endl;
	    TRIGGERSKIPPED++;
	    break;
	  }
  }
}


void TriventTriggered::end()
{  
  _EventWriter->close();
  for(std::map<int,TH1F*>::iterator it=TimeDistribution.begin();it!=TimeDistribution.end();++it)
  {
    Global::out->writeObject("Distribution",it->second);
    delete it->second;
  }
  std::cout << "TriventProcess::end() !! "<<TRIGGERSKIPPED<<" Events skiped"<< std::endl;
  std::cout <<TouchedEvents<<" Events were overlaping "<<"("<<(TouchedEvents*1.0/(TouchedEvents+eventtotal))*100<<"%)"<<std::endl;
  std::cout <<"Total nbr Events : "<<eventtotal<<" : "<<EventsSelected<<" ("<<EventsSelected*1.0/eventtotal*100<<"%)"<< std::endl;
  /*std::cout<<green<<"Total time of the File : "<<total_time_file<<"s Total time of selected events : "<<total_time_by_user<<"s ; Time global ( Usefull Time ) : "<<HistoPlanes[0]->Get_Global_Total_Time()*200e-9<<"s; In percent : "<<HistoPlanes[0]->Get_Global_Total_Time()*100.0*200e-9/total_time_by_user<<"% of usefull time and "<<total_time_by_user*100.0/total_time_file<<"% of time selected in file"<<normal<<std::endl;*/
}
