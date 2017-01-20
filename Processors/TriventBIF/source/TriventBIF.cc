#include "TriventBIF.h"
#include "Utilities.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <cstdint>
#include <algorithm>
#include "Processor.h"
#include "UTIL/LCTOOLS.h"
#include "UTIL/CellIDEncoder.h"
#include <EVENT/LCGenericObject.h>
#include "IMPL/LCCollectionVec.h"
#include "IMPL/LCEventImpl.h"
#include "TH2F.h"
#include <iomanip>
#include <climits>
#include <ctime>
#include "TFile.h"
#include "TTree.h"
#include "IMPL/CalorimeterHitImpl.h"
#include "IMPL/LCRunHeaderImpl.h"
#include "Colors.h"
#include <IMPL/LCParametersImpl.h>
#include "TObject.h"
#include "UTIL/LCTime.h"
#include <fstream>
#include "ReaderFactory.h"
#include "Reader.h"
#include "IO/LCReader.h"
#include "IMPL/LCTOOLS.h"
#include "EVENT/LCRunHeader.h" 
#include <algorithm> 
#include "EVENT/SimCalorimeterHit.h" 
#include "EVENT/CalorimeterHit.h" 
#include "EVENT/RawCalorimeterHit.h" 

TH1F* delay=new TH1F("delay","delay",2000,-1000,1000);

std::vector<int>ScintillatorCoincidence;
unsigned int total_coincidence=0;
unsigned int EventsNoise=0;

unsigned int EventsSelected=0;

TH1F diff("diff","diff",1000,-500,500);

TriventProcessor aTriventProcessor;

TriventProcessor::TriventProcessor() : Processor("TriventProcessorType")
{
  _outFileName="LCIO_clean_run.slcio";
  registerProcessorParameter("LCIOOutputFile","LCIO file",_outFileName,_outFileName);
  _hcalCollections={"XYZFilled"};
  registerInputCollections( LCIO::CALORIMETERHIT,"HCALCollections","HCAL Collection Names",_hcalCollections,_hcalCollections);
  _timeWin = 2;
  registerProcessorParameter("TimeWin" ,"time window = 2 in default",_timeWin ,_timeWin);
  _noiseCut = 7;
  registerProcessorParameter("NoiseCut" ,"noise cut in time spectrum 7 in default",_noiseCut ,_noiseCut);
  _LayerCut = 3;
  registerProcessorParameter("LayerCut" ,"cut in number of layer 3 in default",_LayerCut ,_LayerCut); 
  _TriggerTime=0;
  registerProcessorParameter("TriggerTime" ,"All Events with Time greater than this number will be ignored (0) in case of Triggerless",_TriggerTime ,_TriggerTime);
  _efficiencyFrontScintillator =1;
  registerProcessorParameter("efficiencyFrontScintillator" ,"efficiency Front Scintillator",_efficiencyFrontScintillator ,_efficiencyFrontScintillator);
  _efficiencyBackScintillator =1;
  registerProcessorParameter("efficiencyBackScintillator" ,"efficiency Back Scintillator",_efficiencyBackScintillator ,_efficiencyBackScintillator);
  registerProcessorParameter("Delimiters" ,"Delimiters",_Delimiters,_Delimiters);  
} // end constructor


TriventProcessor::~TriventProcessor() {}

void TriventProcessor::processRunHeader( LCRunHeader* run){}

void TriventProcessor::init()
{ 
  TOTALNUMBERHITSCINTI=0;
  TouchedEvents=0;
  eventtotal=0;
  _EventWriter = LCFactory::getInstance()->createLCWriter() ;
  _EventWriter->setCompressionLevel( 2 ) ;
  _EventWriter->open(_outFileName.c_str(),LCIO::WRITE_NEW) ;
  printParameters();
} // end init method



void TriventProcessor::Loop_On_Scintillator_Collection(LCCollection *col3)
{
  for (int ihit=0; ihit < col3->getNumberOfElements(); ++ihit) 
    {
      EVENT::LCGenericObject* raw_scin = dynamic_cast<EVENT::LCGenericObject* >( col3->getElementAt(ihit)) ;
      _Front_scintillator+=raw_scin->getIntVal(0);
      _Back_scintillator+=raw_scin->getIntVal(1);
      _Both_scintillator+=raw_scin->getIntVal(2);
      if(raw_scin->getIntVal(0)==1||raw_scin->getIntVal(1)==1)
	{
	  ScintillatorCoincidence.push_back(raw_scin->getIntVal(3));
	  total_coincidence+=ScintillatorCoincidence.size();
	  //std::cout<<total_coincidence<<std::endl;
	}    
    } // end for (int ihit=0; ihit < col3->getNumberOfElements(); ++ihit) 
}


void TriventProcessor::processEvent_ProcessCollectionNamed_Scintillator(LCEvent *evtP)
{
  ScintillatorCoincidence.clear();
  LCCollection* col3=nullptr;
  try { col3 = evtP ->getCollection("Scintillator");}
  catch (DataNotAvailableException &e) {return;}
  Loop_On_Scintillator_Collection(col3);
}

void TriventProcessor::processEvent( LCEvent * evtP )
{
  //processEvent_ProcessCollectionNamed_Scintillator (evtP);
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

void TriventProcessor::processCollection(EVENT::LCEvent *evtP,LCCollection* col)
{
  RawHits.clear();
  int numElements = col->getNumberOfElements();
  ////verify if timestamp negative 
  bool HasTimeStampNegative=false;
  for (int ihit=0; ihit < numElements; ++ihit) 
  {
    CalorimeterHit* raw_hit = dynamic_cast<CalorimeterHit*>( col->getElementAt(ihit)) ;
    if (raw_hit != nullptr) 
	  {
	    if(Global::geom->GetDifType(decode(raw_hit)["DIF_Id"])==scintillator||Global::geom->GetDifType(decode(raw_hit)["DIF_Id"])==tcherenkov)continue;
	    if(geom.GetDifNbrPlate(dif_id)==-1) continue;
	    if(raw_hit->getTimeStamp()<0)
	    {
	      //std::vector<unsigned int>b{dif_id,(unsigned int)((raw_hit->getCellID0() & 0xFF00)>>8),(unsigned int)((raw_hit->getCellID0() & 0x3F0000)>>16)}; Negative[b][raw_hit->getTimeStamp()]++;
	      //std::cout<<"TimeStamp <=-1 : "<<raw_hit->getTimeStamp()<<std::endl;
	      if(raw_hit->getTimeStamp()<-1)HasTimeStampNegative=true;         
	    }
	  }
  }
  for (int ihit=0; ihit < numElements; ++ihit) 
  {
    CalorimeterHit* raw_hit = dynamic_cast<CalorimeterHit*>( col->getElementAt(ihit)) ;
    if (raw_hit != nullptr) 
	  {
	    if(Global::geom->GetDifType(decode(raw_hit)["DIF_Id"])==scintillator||Global::geom->GetDifType(decode(raw_hit)["DIF_Id"])==tcherenkov)continue;
	    if(Global::geom->GetDifNbrPlate(decode(raw_hit)["DIF_Id"])==-1)
	    {
	      if(Warningg[dif_id]!=true) 
		    {
		      Warningg[dif_id]=true;
		      std::cout<<"Please add DIF "<<dif_id<<" to your geometry file; I'm Skipping its data."<<std::endl;
		    }
	      continue;
	    }
	    if(_TriggerTime==0 || raw_hit->getTimeStamp()<=_TriggerTime)
	    {
	      if(HasTimeStampNegative==false)
	      {
		      RawHits[raw_hit->getTime()].push_back(raw_hit);
		    }
		  }
    } 
	}
      
  for(std::map< int,int>::iterator itt=Times.begin(); itt!=Times.end(); ++itt) 
	{
	  bool Scintillatorseeittoo=false;
	  for(unsigned int i=0;i!=ScintillatorCoincidence.size();++i)
	  {
	      diff.Fill(ScintillatorCoincidence[i]-itt->first);
	      if(ScintillatorCoincidence[i]-itt->first>=-6&&ScintillatorCoincidence[i]-itt->first<=-3) Scintillatorseeittoo=true;
	  }
	  EventsGrouped.clear();
	  EstimationNoiseBefore.clear();
	  EstimationNoiseAfter.clear();
	  std::map<int,std::vector<RawCalorimeterHit *> >::iterator middle=RawHits.find(itt->first);
	  std::map<int,std::vector<RawCalorimeterHit *> >::iterator after=middle;
	  std::map<int,std::vector<RawCalorimeterHit *> >::iterator before=middle;
	  while(fabs(middle->first-before->first)<=_timeWin && before!=RawHits.begin()) --before;
	  ++before;
	  while(fabs(after->first-middle->first)<=_timeWin && after!=RawHits.end()) ++after;
	  static std::map<int,int> nbrPlanestouched;
	  nbrPlanestouched.clear();
	  for(middle=before; middle!=after; ++middle ) 
	    {  
	      for(int unsigned i=0; i<(middle->second).size(); ++i) 
		{
		  int dif_id=((middle->second)[i])->getCellID0() & 0xFF;
		  nbrPlanestouched[geom.GetDifNbrPlate(dif_id)]++;
		}
	    }
	  if(Scintillatorseeittoo)
	    {
	      //////////////////////////Just Scintillator
	      //std::cout<<red<<counttt++<<normal<<std::endl;
	      
	  for(middle=before; middle!=after;++middle ) 
		{
		  EventsGroupedScin.insert(EventsGroupedScin.end(),middle->second.begin(),middle->second.end());
		}
	      CalculateEfficiencyScinti(EventsGroupedScin);
	      
	      if(nbrPlanestouched.size()>=(unsigned int)(_LayerCut)) 
		{
		  static unsigned int EventsSelectedt=0;
		  EventsSelectedt++;
		  LCEventImpl*  evtt = new LCEventImpl() ;
		  LCCollectionVec* col_eventt = new LCCollectionVec(LCIO::CALORIMETERHIT);
		  CellIDEncoder<CalorimeterHitImpl> cdt( "I:8,J:7,K:10,Dif_id:8,Asic_id:6,Chan_id:7" ,col_eventt) ;
		  col_eventt->setFlag(col_eventt->getFlag()|( 1 << LCIO::RCHBIT_LONG));
		  col_eventt->setFlag(col_eventt->getFlag()|( 1 << LCIO::RCHBIT_TIME));
		  FillIJK(EventsGroupedScin, col_eventt,cdt,0);
		  evtt->addCollection(col_eventt, "SDHCAL_HIT_SC");
		  evtt->setEventNumber(EventsSelectedt);
		  //evtt->setTime(evtP->getTimeStamp());
		  evtt->setRunNumber(evtP->getRunNumber());
		  _EventWriter->writeEvent( evtt ) ;
		  delete evtt;
		}
	      EventsGroupedScin.clear();
	    } //end if(Scintillatorseeittoo)
	}//end for(std::map< int,int>::iterator itt=Times.begin(); itt!=Times.end(); ++itt) 
}//end processCollection

void TriventProcessor::end()
{  
  _EventWriter->close();
  std::cout << "TriventProcess::end() !! "<<_trig_count<<" Events Trigged"<< std::endl;
  std::cout <<"Total nbr Events : "<<eventtotal<<" Events with nbr of plates >="<<_LayerCut<<" : "<<EventsSelected<<" ("<<EventsSelected*1.0/eventtotal*100<<"%)"<< std::endl;
  for(std::map<int,bool>::iterator it=Warningg.begin(); it!=Warningg.end(); it++) 
  {
    std::cout<<red<<"REMINDER::Data from Dif "<<it->first<<" are skipped !"<<normal<<std::endl;
  }
  if(EffiwithDiscri.size()!=0)
  {
    TH1F* counterScinti=new TH1F("counterScinti","counterScinti",6,0,6);
    counterScinti->Fill(0.5,_Front_scintillator);
    counterScinti->Fill(1.5,_Back_scintillator);
    counterScinti->Fill(2.5,_Both_scintillator);
    counterScinti->Fill(3.5,_Front_scintillator*1.0/_efficiencyFrontScintillator);
    counterScinti->Fill(4.5,_Back_scintillator*1.0/_efficiencyBackScintillator);
    counterScinti->Fill(5.5,_Both_scintillator*1.0/(_efficiencyFrontScintillator*_efficiencyBackScintillator));
    counterScinti->Write();
    counterScinti->Scale(1.0/(HistoPlanes[0]->Get_Global_Total_Time()*200e-9));
    counterScinti->Write("EstimatedRateScinti");
    std::cout<<green<<"Counter Front scintillator : "<<_Front_scintillator<<" Counter Back scintillator : "<< _Back_scintillator<<" Counter Both scintillator : "<<_Both_scintillator<<"  "<<normal<<std::endl;
    std::cout<<green<<"Counter Front scintillator : "<<_Front_scintillator*1.0/_efficiencyFrontScintillator<<" Counter Back scintillator : "<< _Back_scintillator*1.0/_efficiencyBackScintillator<<" Counter Both scintillator : "<<_Both_scintillator*1.0/(_efficiencyFrontScintillator*_efficiencyBackScintillator)<<"  "<<normal<<std::endl;
    std::cout<<green<<"Efficiency Calculated with Scintillators : "<<normal<<std::endl;
    for(unsigned int i=0;i!=EffiwithDiscri.size();++i)
	  {
	    std::cout<<"Plate : "<<i+1<<"  "<<EffiwithDiscri[i]*100.0/total_coincidence<<"  "<<normal;
	  }
    std::cout<<std::endl;
    ofstream fichier;
    fichier.open("ResultsScinti.txt", ios::out | ios::app);  //déclaration du flux et ouverture du fichier
    if(fichier) 
    { // si l'ouverture a réussi
      fichier<<_NbrRun<<"   ";
      for(unsigned int i=0;i!=EffiwithDiscri.size();++i)
	    {
	      fichier<<"Plate : "<<i+1<<EffiwithDiscri[i]<<"  "<<total_coincidence<<" Efficiency :  "<<EffiwithDiscri[i]*100.0/total_coincidence<<" In the zone : "<<EffiwithDiscri[i]*100.0/counttt;
	    }
      fichier<<std::endl;
      fichier.close();  // on referme le fichier
    } //end if(fichier) 
  }//end if(EffiwithDiscri.size()!=0)
  if(Negative.size()!=0)
  {
    std::cout<<red<<"WARNING !!! : Negative Value(s) of timeStamp found. They are written in Negative_Values.txt"<<normal<<std::endl;
    std::ofstream fileNeg( "Negative_Values"+std::to_string(_NbrRun)+".txt", std::ios_base::out ); 
    for(std::map<std::vector<unsigned int>,std::map< int, int>>::iterator it=Negative.begin();it!=Negative.end();++it)
    {
	    fileNeg<<"Dif_Id : "<<it->first[0]<<" Asic_Id : "<<it->first[1]<<" Channel_Id : "<<it->first[2];
	    for(std::map< int, int>::iterator itt=it->second.begin();itt!=it->second.end();++itt)
	    {
	      fileNeg<<" Value : "<<itt->first<<" - "<<itt->second<<" Times; ";
	    }
	    fileNeg<<std::endl;
    }
    fileNeg.close();
  }
}//end end()


