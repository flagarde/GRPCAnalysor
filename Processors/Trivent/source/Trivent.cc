#include "Trivent.h"
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

void Trivent::FillTimes()
{
  bool eraseFirst=false;
  bool nextHasBeenErased=false;
  for (std::map<int,int>::iterator it=Times.begin(); it!= Times.end(); ++it) 
  {
    if (nextHasBeenErased) --it;
    nextHasBeenErased=false;
    bool eraseIt=(it->second<_noiseCut);
    if (!eraseIt) 
    {
      std::map<int,int>::iterator itnext=it;
      ++itnext;
      if (fabs(itnext->first-it->first)<=_timeWin) 
      {
        if (itnext->second >= it->second)	eraseIt=true;
        else 
        {
          Times.erase(itnext);
          nextHasBeenErased=true;
        }
      }
    }
    if (eraseIt) 
    {
      std::map<int,int>::iterator itprev=it;
      --itprev;
      if (it == Times.begin()) eraseFirst=true;
      else 
      {
        Times.erase(it);
        it=itprev;
      }
    }
  }
  if (eraseFirst) Times.erase(Times.begin());
  std::set<int>touched;
  for(std::map< int,int>::iterator firstit=Times.begin(); firstit!=Times.end(); ++firstit) 
  {
    eventtotal++;
    if(firstit!=--(Times.end())) 
    {
      std::map< int,int>::iterator secondit=firstit;
      secondit++;
      if(secondit->first-firstit->first<2*_timeWin)
      {
       // streamlog_message(DEBUG,std::cout<<magenta<<secondit->first<<"  "<<firstit->first<<normal<<std::endl;touched.insert(firstit->first);touched.insert(secondit->first);,"";);
      } 
      // streamlog_message(DEBUG,std::cout<<green<<secondit->first<<"  "<<firstit->first<<normal<<std::endl; ,"";);
    }
  }
  TouchedEvents+=touched.size();
  for(std::set< int>::iterator it=touched.begin(); it!=touched.end(); ++it) 
  {
    Times.erase(*it);
  }
  //streamlog_message(MESSAGE0,if(touched.size()!=0)std::cout<<touched.size()<<" Events are touched !"<<std::endl; ,"";);
}

Trivent aTrivent;

Trivent::Trivent() : Processor("Trivent")
{
  _hcalCollections={"XYZFilled"};
  registerInputCollections( LCIO::CALORIMETERHIT,"HCALCollections","HCAL Collection Names",_hcalCollections,_hcalCollections);
  _timeWin = 2;
  registerProcessorParameter("TimeWin" ,"time window = 2 in default",_timeWin ,_timeWin);
  _noiseCut = 7;
  registerProcessorParameter("NoiseCut" ,"noise cut in time spectrum 7 in default",_noiseCut ,_noiseCut);
  _LayerCut = 3;
  registerProcessorParameter("LayerCut" ,"cut in number of layer 3 in default",_LayerCut ,_LayerCut); 
  _Time_from_Track=4;
  registerProcessorParameter("TimeFromTrack" ,"Time from track",_Time_from_Track,_Time_from_Track);
} // end constructor

Trivent::~Trivent() {}

void Trivent::processRunHeader( LCRunHeader* run){}

void Trivent::init()
{ 
  printParameters();
}

void Trivent::processEvent( LCEvent * evtP )
{
  for(unsigned int i=0; i< _hcalCollections.size(); i++) 
  {
    try 
	  {
	    LCCollectionVec* clu1 = new LCCollectionVec(LCIO::CLUSTER);
	    LCCollectionVec* clu2 = new LCCollectionVec(LCIO::CLUSTER);
	    LCCollectionVec* clu3 = new LCCollectionVec(LCIO::CLUSTER);
	    LCCollection* col = evtP ->getCollection(_hcalCollections[i].c_str());
      int numElements = col->getNumberOfElements();
      CellIDDecoder<CalorimeterHit>decode(col);
      for (int ihit=0; ihit < numElements; ++ihit) 
      {
        CalorimeterHit *raw_hit = dynamic_cast<CalorimeterHit*>( col->getElementAt(ihit)) ;
        if (raw_hit != nullptr) 
	      {
	        if(Global::geom->GetDifType(decode(raw_hit)["DIF_Id"])==scintillator||Global::geom->GetDifType(decode(raw_hit)["DIF_Id"])==tcherenkov)continue;
	        if(Global::geom->GetDifNbrPlate(decode(raw_hit)["DIF_Id"])==-1) continue;
		      Times[raw_hit->getTime()]++;
		      RawHits[raw_hit->getTime()].push_back(raw_hit);
        } 
	    } 
      FillTimes();
      for(std::map< int,int>::iterator itt=Times.begin(); itt!=Times.end(); ++itt) 
	    {
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
	      int val_middle=middle->first;
	      int d=_Time_from_Track;
	      int win=_TimeWin_Noise;
	      std::map<int,std::vector<CalorimeterHit *> >::iterator debut_avant=std::find_if(RawHits.begin(), before, [val_middle,d,win] (const      std::pair<int, std::vector<CalorimeterHit *>>& v)->bool { if(v.first>=val_middle-d-win)return 1;else return 0; } );
	      if(debut_avant==RawHits.end())debut_avant=RawHits.begin();

	      std::map<int,std::vector<CalorimeterHit *> >::iterator debut_apres=std::find_if(after, RawHits.end(), [val_middle,d,win] (const std::pair<int, std::vector<CalorimeterHit *>>& v)->bool { if(v.first>=val_middle+d+win)return 1;else return 0; } );
	      if(nbrPlanestouched.size()>=(unsigned int)(_LayerCut)) 
	      {
	        EventsSelected++;
	        ClusterImpl* cluster1=new ClusterImpl();
	        for(std::map<int,std::vector<CalorimeterHit *> >::iterator it=debut_avant;it!=before;++it) 
		      {
		        if(abs(it->first-(val_middle-d))<=_TimeWin_Noise)
		        {
		          for(std::vector<CalorimeterHit *>::iterator itt=it->second.begin();itt!=it->second.end();++itt)
		          {
		            cluster1->addHit(*itt,1.0);
		          }
		        }
		      }
		      clu1->addElement(cluster1);
		      ClusterImpl* cluster2=new ClusterImpl();
	        for(std::map<int,std::vector<CalorimeterHit *> >::iterator it=after;it!=debut_apres;++it) 
		      {
		        if(abs(it->first-(val_middle-d))<=_TimeWin_Noise)
		        {
		          for(std::vector<CalorimeterHit *>::iterator itt=it->second.begin();itt!=it->second.end();++itt)
		          {
		            cluster2->addHit(*itt,1.0);
		          }
		        }
		      }
		      clu2->addElement(cluster2);
	        ClusterImpl* cluster3=new ClusterImpl();
	        for(middle=before; middle!=after; ) 
		      {
		        for(std::vector<CalorimeterHit *>::iterator itt=middle->second.begin();itt!=middle->second.end();++itt)
		        {
		          cluster3->addHit(*itt,1.0);
		        }
		        RawHits.erase(middle++);
		      }
		      clu3->addElement(cluster3);
	      }
	    }
	    Times.clear();
	    RawHits.clear();
	    evtP->addCollection(clu3, "TRIVENT");
	    evtP->addCollection(clu2, "NOISE_AFTER");
	    evtP->addCollection(clu1, "NOISE_BEFORE");
	  }
    catch (DataNotAvailableException &e)
	  {
	    std::cout << "TRIGGER SKIPED ..."<<std::endl;
	    _trig_count++;
	    break;
	  }
  } 
}


void Trivent::end()
{  
  std::cout << "TriventProcess::end() !! "<<_trig_count<<" Events Trigged"<< std::endl;
  std::cout <<TouchedEvents<<" Events were overlaping "<<"("<<(TouchedEvents*1.0/(TouchedEvents+eventtotal))*100<<"%)"<<std::endl;
  std::cout <<"Total nbr Events : "<<eventtotal<<" Events with nbr of plates >="<<_LayerCut<<" : "<<EventsSelected<<" ("<<EventsSelected*1.0/eventtotal*100<<"%)"<< std::endl;
  /*std::cout<<green<<"Total time of the File : "<<total_time_file<<"s Total time of selected events : "<<total_time_by_user<<"s ; Time global ( Usefull Time ) : "<<HistoPlanes[0]->Get_Global_Total_Time()*200e-9<<"s; In percent : "<<HistoPlanes[0]->Get_Global_Total_Time()*100.0*200e-9/total_time_by_user<<"% of usefull time and "<<total_time_by_user*100.0/total_time_file<<"% of time selected in file"<<normal<<std::endl;*/
}
