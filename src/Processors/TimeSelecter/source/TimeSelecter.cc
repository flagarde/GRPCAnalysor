#include "TimeSelecter.h"
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
#include "IMPL/LCRunHeaderImpl.h"
#include <IMPL/LCParametersImpl.h>
#include <fstream>
#include "EVENT/LCRunHeader.h" 
#include <algorithm> 
#include "EVENT/CalorimeterHit.h" 
#include "IMPL/ClusterImpl.h"
TimeSelecter aTimeSelecter;

TimeSelecter::TimeSelecter() : Processor("TimeSelecter")
{
  _outFileName="";
  registerProcessorParameter("LCIOOutputFile","LCIO file",_outFileName,_outFileName);
  _hcalCollections={"XYZFilled"};
  registerInputCollections( LCIO::CALORIMETERHIT,"HCALCollections","HCAL Collection Names",_hcalCollections,_hcalCollections);
  _noiseCut = -1;
  registerProcessorParameter("NoiseCut" ,"Number of hits maximum per DIFs and events (-1) to ignore this parameter",_noiseCut ,_noiseCut);
  _TriggerTimeLow=-1;
  registerProcessorParameter("TriggerTimeLow" ,"All Events with Time lower than this number will be ignored (-1) in case of Triggerless",_TriggerTimeLow,_TriggerTimeLow);
  _TriggerTimeHigh=-1;
  registerProcessorParameter("TriggerTimeHigh" ,"All Events with Time higher than this number will be ignored (-1) in case of Triggerless",_TriggerTimeHigh,_TriggerTimeHigh);
} // end constructor

TimeSelecter::~TimeSelecter() {}

void TimeSelecter::processRunHeader( LCRunHeader* run){}

void TimeSelecter::init()
{ 
  std::string namee="";
  if(_outFileName=="")namee="TriventTriggered_"+std::to_string(Global::number)+".slcio";
  else namee=_outFileName;
  _EventWriter = LCFactory::getInstance()->createLCWriter() ;
  _EventWriter->setCompressionLevel( 2 ) ;
  _EventWriter->open(namee.c_str(),LCIO::WRITE_NEW) ;
  if(_TriggerTimeLow==-1)_TriggerTimeLow=0;
  if(_TriggerTimeHigh==-1)_TriggerTimeHigh=std::numeric_limits<int>::max();
  if(_noiseCut==-1)_noiseCut=std::numeric_limits<int>::max();
  printParameters();
  NumberOfEventsEfficientDIF.clear();
  NumberOfEventsEfficientPlan.clear();
  MinMaxTime=std::pair<double,double>(0.,0.);
  MinMaxTimeRejected=std::pair<double,double>(0.,0.);
  for(unsigned int i=0;i!=Global::geom->GetNumberPlates();++i)
  {
    int type=Global::geom->GetDifType(Global::geom->GetDifsInPlane(i)[0]);
    if(type!=temporal&&type!=scintillator&&type!=tcherenkov&&type!=bif)
    {
      std::string name="Time Distribution in plate nbr "+std::to_string(i+1);
      std::string name2="Hits Distribution in plate nbr "+std::to_string(i+1);
      std::string name3="Hits Distribution in plate nbr "+std::to_string(i+1)+" out of trigger time";
      std::string name4="Time Distribution in plate nbr "+std::to_string(i+1)+" out of trigger time";
      TimeDistribution[i]=new TH1F(name.c_str(),name.c_str(),100000,0,100000);
      TimeDistributionRejected[i]=new TH1F(name4.c_str(),name4.c_str(),100000,0,100000);
      HitsDistribution[i]=new TH2F(name2.c_str(),name2.c_str(),35,0,35,40,0,40);
      HitsDistributionRejected[i]=new TH2F(name3.c_str(),name3.c_str(),35,0,35,40,0,40);
    }
  } 
}

void TimeSelecter::processEvent( LCEvent * evtP )
{
  MinMaxTime.first=_TriggerTimeLow;
  if(_TriggerTimeHigh!=std::numeric_limits<int>::max()) MinMaxTime.second=_TriggerTimeHigh;
  else MinMaxTime.second=0;
  for(unsigned int i=0; i< _hcalCollections.size(); i++) 
  {
    try 
	  {
	    LCCollection* col = evtP ->getCollection(_hcalCollections[i].c_str());
	    if(col==nullptr) continue;
      CellIDDecoder<CalorimeterHit>decode(col);
      LCEventImpl*  evtt = new LCEventImpl() ;
      LCCollectionVec* clu1 = new LCCollectionVec(LCIO::CALORIMETERHIT);
      LCCollectionVec* clu2 = new LCCollectionVec(LCIO::CALORIMETERHIT);
      CellIDEncoder<CalorimeterHitImpl> cdt( "I:8,J:7,K:10,DIF_Id:8,Asic_Id:6,Channel:7" ,clu1) ;
      CellIDEncoder<CalorimeterHitImpl> cdt2( "I:8,J:7,K:10,DIF_Id:8,Asic_Id:6,Channel:7" ,clu2) ;
	    clu1->setFlag(clu1->getFlag()|( 1 << LCIO::RCHBIT_LONG));
	    clu1->setFlag(clu1->getFlag()|( 1 << LCIO::RCHBIT_TIME));
	    clu2->setFlag(clu2->getFlag()|( 1 << LCIO::RCHBIT_LONG));
	    clu2->setFlag(clu2->getFlag()|( 1 << LCIO::RCHBIT_TIME));
	    bool selected=true;
      for (int ihit=0; ihit < col->getNumberOfElements(); ++ihit) 
      {
        CalorimeterHit* raw_hit = dynamic_cast<CalorimeterHit*>( col->getElementAt(ihit)) ;
        if (raw_hit != nullptr) 
	      {
	        int nbrplate=Global::geom->GetDifNbrPlate(decode(raw_hit)["DIF_Id"]);
	        if(Global::geom->GetDifType(decode(raw_hit)["DIF_Id"])==scintillator||Global::geom->GetDifType(decode(raw_hit)["DIF_Id"])==tcherenkov||Global::geom->GetDifType(decode(raw_hit)["DIF_Id"])==bif)continue;
	        if(Global::geom->GetDifNbrPlate(decode(raw_hit)["DIF_Id"])==-1) continue;
	        if(_TriggerTimeLow<=raw_hit->getTime()&&raw_hit->getTime()<=_TriggerTimeHigh)
	        {
	          SelectedHits[decode(raw_hit)["DIF_Id"]].push_back(raw_hit);
	          if(_TriggerTimeHigh==std::numeric_limits<int>::max())if(MinMaxTime.second<raw_hit->getTime())MinMaxTime.second=raw_hit->getTime();
	          if(nbrplate<Global::geom->GetNumberPlates())
	          {
	            HitsDistribution[nbrplate]->Fill(decode(raw_hit)["I"],decode(raw_hit)["J"]);
	            TimeDistribution[nbrplate]->Fill(raw_hit->getTime());
	          }
		      }
		      else
		      {
	            RejectedHits[decode(raw_hit)["DIF_Id"]].push_back(raw_hit);
	            if(MinMaxTimeRejected.second<raw_hit->getTime())MinMaxTimeRejected.second=raw_hit->getTime();
	            if(nbrplate<Global::geom->GetNumberPlates())
	            {
	              TimeDistributionRejected[nbrplate]->Fill(raw_hit->getTime());
	              HitsDistributionRejected[nbrplate]->Fill(decode(raw_hit)["I"],decode(raw_hit)["J"]);
	            }
		      }
        } 
	    }
	    for(std::map< int,std::vector<CalorimeterHit*> >::iterator it=SelectedHits.begin();it!=SelectedHits.end();++it)
	    {
	      if((it->second).size()>=(unsigned int)(_noiseCut))
	      {
	        selected=false;
	        break;
	      }
	      else selected=true;
	    }
	    if(selected==true)
	    {
	      EventsSelected++;
	      std::map<int,bool> HASHITS; 
	      for(std::map< int,std::vector<CalorimeterHit*> >::iterator it=SelectedHits.begin();it!=SelectedHits.end();++it)
	      {
	        if(it->second.size()!=0)
	        {
	          NumberOfEventsEfficientDIF[it->first]++;
	          HASHITS[Global::geom->GetDifNbrPlate(it->first)]=true;
	        }
	        for(unsigned int o=0;o!=it->second.size();++o)
	        {
	          clu1->addElement(it->second[o]);
	        }
	      }
	      for(std::map<int,bool>::iterator iu=HASHITS.begin();iu!=HASHITS.end();++iu)
	      {
	        NumberOfEventsEfficientPlan[iu->first]++;
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
        HASHITS.clear();
      }
      SelectedHits.clear();
      RejectedHits.clear();
	  }
    catch (DataNotAvailableException &e)
	  {
	    std::cout << "collection skipped ..."<<std::endl;
	    TRIGGERSKIPPED++;
	    break;
	  }
	  
  }
  TotalTime+=(MinMaxTime.second-MinMaxTime.first)*200e-9;
  TotalTimeRejected+=((MinMaxTimeRejected.second-MinMaxTimeRejected.first)-TotalTime)*200e-9;
  MinMaxTimeRejected=std::pair<double,double>(0.,0.);
  MinMaxTime=std::pair<double,double>(0.,0.);
}


void TimeSelecter::end()
{  
  std::vector<std::string>NameBin;
  std::vector<double>ValueBin;
  for(unsigned int i=0;i!=Global::geom->GetNumberPlates();++i)
  {
    for(unsigned int o=0;o!=Global::geom->GetDifsInPlane(i).size();++o)
    {
      NameBin.push_back("DIF "+std::to_string(Global::geom->GetDifsInPlane(i)[o]));
      Global::HG("Efficiency vs HV",i+1,Global::geom->GetDifsInPlane(i)[o],1,1,1).Fill(Global::eloginf->HV[Global::geom->GetDifsInPlane(i)[o]],NumberOfEventsEfficientDIF[Global::geom->GetDifsInPlane(i)[o]]*100.0/EventsSelected);
      ValueBin.push_back(NumberOfEventsEfficientDIF[Global::geom->GetDifsInPlane(i)[o]]*100.0/EventsSelected);
    }
    NameBin.push_back("Plate Nbr"+std::to_string(i+1));
    ValueBin.push_back(NumberOfEventsEfficientPlan[i+1]*100.0/EventsSelected);
  }
  TH1F* Efficiencies=new TH1F("Efficiencies","Efficiencies",NameBin.size(),0,NameBin.size());
  for(unsigned int i=0;i!=NameBin.size();++i)
  {
    Efficiencies->Fill(i,ValueBin[i]);
    Efficiencies->GetXaxis()->SetBinLabel(i+1,NameBin[i].c_str());
  }
  Efficiencies->GetYaxis()->SetRangeUser(0.0,101.0);
  Efficiencies->GetYaxis()->SetTitle("Efficiency (%)");
  Efficiencies->GetYaxis()->SetNdivisions(000510);
  Global::out->writeObject("Efficiencies",Efficiencies);
  delete Efficiencies;
  _EventWriter->close();
  for(std::map<int,TH1F*>::iterator it=TimeDistribution.begin();it!=TimeDistribution.end();++it)
  {
    Global::out->writeObject("TimeDistribution",it->second);
    delete it->second;
  }
  for(std::map<int,TH1F*>::iterator it=TimeDistributionRejected.begin();it!=TimeDistributionRejected.end();++it)
  {
    Global::out->writeObject("TimeDistributionRejected",it->second);
    
    delete it->second;
  }
  for(std::map<int,TH2F*>::iterator it=HitsDistribution.begin();it!=HitsDistribution.end();++it)
  {
    Global::out->writeObject("HitsDistribution",it->second);
    (it->second)->Scale(1.0/(TotalTime*Global::geom->GetAreaOneCellPlane(it->first)));
    std::cout<<Global::geom->GetAreaOneCellPlane(it->first)<<std::endl;
    Global::out->writeObject("HitsDistribution/Scaled",it->second);
    delete it->second;
  }
  for(std::map<int,TH2F*>::iterator it=HitsDistributionRejected.begin();it!=HitsDistributionRejected.end();++it)
  {
    Global::out->writeObject("HitsDistributionRejected",it->second);
    (it->second)->Scale(1.0/(TotalTimeRejected*Global::geom->GetAreaOneCellPlane(it->first)));
    Global::out->writeObject("HitsDistributionRejected/Scaled",it->second);
    delete it->second;
  }
  std::cout << "TriventProcess::end() !! "<<TRIGGERSKIPPED<<" Events skiped"<< std::endl;
  std::cout <<TouchedEvents<<" Events were overlaping "<<"("<<(TouchedEvents*1.0/(TouchedEvents+eventtotal))*100<<"%)"<<std::endl;
  std::cout <<"Total nbr Events : "<<eventtotal<<" : "<<EventsSelected<<" ("<<EventsSelected*1.0/eventtotal*100<<"%)"<< std::endl;
}
