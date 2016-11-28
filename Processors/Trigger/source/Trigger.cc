#include "Trigger.h"
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
#include <climits>
#include "Utilities.h"
#include "IMPL/LCRunHeaderImpl.h"
#include <IMPL/LCParametersImpl.h>
#include <fstream>
#include "EVENT/LCRunHeader.h" 
#include <algorithm> 
#include "EVENT/CalorimeterHit.h" 
#include "IMPL/ClusterImpl.h"

Trigger aTrivent;

Trigger::Trigger() : Processor("Trigger")
{
  _hcalCollections={"XYZFilled"};
  registerInputCollections( LCIO::CALORIMETERHIT,"HCALCollections","HCAL Collection Names",_hcalCollections,_hcalCollections);
  _LayerCut = 3;
  registerProcessorParameter("LayerCut" ,"cut in number of layer 3 in default",_LayerCut ,_LayerCut); 
  _Time_before=4;
  registerProcessorParameter("TimeBefore" ,"Time before BIF",_Time_before,_Time_before);
  _Time_after=4;
  registerProcessorParameter("TimeAfter" ,"Time after BIF",_Time_after,_Time_after);
} // end constructor

Trigger::~Trigger() {}

void Trigger::processRunHeader( LCRunHeader* run){}

void Trigger::init()
{ 
  //printParameters();
}

void Trigger::processEvent( LCEvent * evtP )
{
  for(unsigned int i=0; i< _hcalCollections.size(); i++) 
  {
    try 
	  {
	    LCCollection* col = evtP ->getCollection(_hcalCollections[i].c_str());
      processCollection(evtP,col);
	  }// end try block
    catch (DataNotAvailableException &e)
	  {
	    std::cout << "TRIGGER SKIPED ..."<<std::endl;
	    _trig_count++;
	    break;
	  }
  } //end  for(unsigned int i=0; i< _hcalCollections.size(); i++) 
} //end processEvent

void Trigger::processCollection(EVENT::LCEvent *evtP,LCCollection* col)
{
  int numElements = col->getNumberOfElements();
  CellIDDecoder<CalorimeterHit>decode(col);
  for (int ihit=0; ihit < numElements; ++ihit) 
  {
    CalorimeterHit *raw_hit = dynamic_cast<CalorimeterHit*>( col->getElementAt(ihit)) ;
    if (raw_hit != nullptr) 
	  {
	    if(Global::geom->GetDifType(decode(raw_hit)["DIF_Id"])==scintillator||Global::geom->GetDifType(decode(raw_hit)["DIF_Id"])==tcherenkov)
	    {
	      BIFHits[raw_hit->getTime()]=raw_hit->getEnergy();
	    }
	    else if(Global::geom->GetDifNbrPlate(decode(raw_hit)["DIF_Id"])==-1) continue;
	    else
	    {
	      RawHits[raw_hit->getTime()].push_back(raw_hit);
	    }
		  //Times[raw_hit->getTime()]++;
    } 
	} 
  //FillTimes();
  LCCollectionVec* clu1 = new LCCollectionVec(LCIO::CLUSTER);
	CellIDEncoder<CalorimeterHit> cd( "I:8,J:7,K:10,DIF_Id:8,Asic_Id:6,Channel:7",clu1) ;
	clu1->setFlag(clu1->getFlag()|( 1 << LCIO::RCHBIT_LONG));
	clu1->setFlag(clu1->getFlag()|( 1 << LCIO::RCHBIT_TIME));
	clu1->setFlag(clu1->getFlag()|( 1 << LCIO::CHBIT_ID1));
	clu1->setFlag(clu1->getFlag()|( 1 << LCIO::RCHBIT_ENERGY_ERROR));
	LCCollectionVec* clu2 = new LCCollectionVec(LCIO::CLUSTER);
	CellIDEncoder<CalorimeterHit> cd2( "I:8,J:7,K:10,DIF_Id:8,Asic_Id:6,Channel:7",clu2) ;
	clu2->setFlag(clu1->getFlag()|( 1 << LCIO::RCHBIT_LONG));
	clu2->setFlag(clu1->getFlag()|( 1 << LCIO::RCHBIT_TIME));
	clu2->setFlag(clu1->getFlag()|( 1 << LCIO::CHBIT_ID1));
	clu2->setFlag(clu1->getFlag()|( 1 << LCIO::RCHBIT_ENERGY_ERROR));
	LCCollectionVec* clu3 = new LCCollectionVec(LCIO::CLUSTER);
	CellIDEncoder<CalorimeterHit> cd3( "I:8,J:7,K:10,DIF_Id:8,Asic_Id:6,Channel:7",clu3) ;
	clu3->setFlag(clu3->getFlag()|( 1 << LCIO::RCHBIT_LONG));
	clu3->setFlag(clu3->getFlag()|( 1 << LCIO::RCHBIT_TIME));
	clu3->setFlag(clu3->getFlag()|( 1 << LCIO::CHBIT_ID1));
	clu3->setFlag(clu3->getFlag()|( 1 << LCIO::RCHBIT_ENERGY_ERROR));
	bool has_1=false;
	bool has_2=false;
	bool has_3=false;
  for(std::map< int,int>::iterator itt=BIFHits.begin(); itt!=BIFHits.end(); ++itt) 
	{
	  static std::map<int,int> nbrPlanestouched;
	  nbrPlanestouched.clear();
	  ClusterImpl* cluster=new ClusterImpl();
	  for(std::map< int,std::vector<CalorimeterHit*> >::iterator it=RawHits.begin();it!=RawHits.end();++it)
	  {
	    if((it->first-itt->first)<=_Time_after&&(it->first-itt->first)>=0&&(it->first-itt->first)>=-_Time_before&&(it->first-itt->first)<=0)
	    {
	      
	      for(unsigned int p=0;p!=it->second.size();++p) 
	      {
	        nbrPlanestouched[Global::geom->GetDifNbrPlate(decode((it->second)[p])["DIF_Id"])]++;
	        cluster->addHit(it->second[p],1.0);
	      }
	      if(nbrPlanestouched.size()>=_LayerCut)
	      {
	        if(itt->second==1)
	        {
	          has_1=true;
	          clu1->addElement(cluster);
	        }
	        else if (itt->second==2)
	        {
	          has_2=true;
	          clu2->addElement(cluster);
	        }
	        else if (itt->second==3)
	        {
	          has_3=true; 
	          clu3->addElement(cluster);
	        }
	      }
	    }
	  }
	}
	if(has_3==true)evtP->addCollection(clu3, "TRIGERRED_BOTH");
	if(has_2==true)evtP->addCollection(clu2, "TRIGERRED_SECOND");
	if(has_1==true)evtP->addCollection(clu1, "TRIGERRED_FIRST");
}//end processCollection

void Trigger::end()
{  
  std::cout << "Trigger::end() !! "<<_trig_count<<" Events Trigged"<< std::endl;
  std::cout <<TouchedEvents<<" Events were overlaping "<<"("<<(TouchedEvents*1.0/(TouchedEvents+eventtotal))*100<<"%)"<<std::endl;
  /*std::cout <<"Total nbr Events : "<<eventtotal<<" Events with nbr of plates >="<<_LayerCut<<" : "<<EventsSelected<<" ("<<EventsSelected*1.0/eventtotal*100<<"%)"<< std::endl;
  std::cout<<green<<"Total time of the File : "<<total_time_file<<"s Total time of selected events : "<<total_time_by_user<<"s ; Time global ( Usefull Time ) : "<<HistoPlanes[0]->Get_Global_Total_Time()*200e-9<<"s; In percent : "<<HistoPlanes[0]->Get_Global_Total_Time()*100.0*200e-9/total_time_by_user<<"% of usefull time and "<<total_time_by_user*100.0/total_time_file<<"% of time selected in file"<<normal<<std::endl;*/
}
