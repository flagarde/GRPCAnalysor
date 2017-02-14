#include "Clusterisation.h"
#include "Global.h"
#include <iostream>
#include <string>
#include "Processor.h"
#include "IMPL/LCCollectionVec.h"
#include <cmath>
#include "Processor.h"
#include "UTIL/LCTOOLS.h"
#include "UTIL/CellIDEncoder.h"
#include "EVENT/LCGenericObject.h"
#include "IMPL/LCCollectionVec.h"
#include "IMPL/LCEventImpl.h"
#include "Utilities.h"
#include "IMPL/CalorimeterHitImpl.h"
#include "IMPL/LCRunHeaderImpl.h"
#include "Colors.h"
#include "IMPL/LCParametersImpl.h"
#include "EVENT/CalorimeterHit.h" 
#include "EVENT/RawCalorimeterHit.h"
#include "UTIL/CellIDDecoder.h"
using namespace lcio ;

Clusterisation aClusterisation;

Clusterisation::Clusterisation():Processor("Clusterisation")
{
  _hcalCollections={"XYZFilled"};
  registerInputCollections( LCIO::CALORIMETERHIT,"HCALCollections","HCAL Collection Names",_hcalCollections,_hcalCollections);
  poundGain=true;
  registerProcessorParameter("poundGain", "pounderate hit by gain",poundGain,poundGain);
  poundThreshold=true;
  registerProcessorParameter("poundThreshold", "pounderate hit by Threshold",poundThreshold,poundThreshold);
  m_neighbourTimeDistance=1;
  registerProcessorParameter("neighbourTimeDistance", "neighbourTimeDistance in clocktick(s)",m_neighbourTimeDistance,m_neighbourTimeDistance);
  m_neighbourStripDistance=1;
  registerProcessorParameter("neighbourStripDistance", "neighbourStripDistance",m_neighbourStripDistance,m_neighbourStripDistance);
  m_neighbourPadDistance=1;
  registerProcessorParameter("neighbourPadDistance", "neighbourPadDistance",m_neighbourPadDistance,m_neighbourPadDistance);
  m_padJunctionByCorner=true;
  registerProcessorParameter("padJunctionByCorner", "padJunctionByCorner",m_padJunctionByCorner,m_padJunctionByCorner);
} 

void Clusterisation::processRunHeader( LCRunHeader* run){}

void Clusterisation::init()
{ 
  printParameters();
  if((poundThreshold==true||poundGain==true)&&Global::Histogram==nullptr)
  {
    std::cout<<red<<"You ask (Threshold|Gain) ponderations but these informations was inaccessible"<<normal<<std::endl;
    std::cout<<red<<"Database is not available or the xml of the SlowControl is not here or the elog xml is not there !!!"<<normal<<std::endl;
  }
  hists.Add("TH1","Plane","Cluster_Size",50,0.,50.);
  hists.setRolling("Cluster_Size",false);
  hists.List();
}

void Clusterisation::processEvent( LCEvent * evtP )
{
  for(unsigned int i=0; i< _hcalCollections.size(); i++) 
  {
    try 
	  {
      LCCollection* col = evtP ->getCollection(_hcalCollections[i].c_str());
      CellIDDecoder<CalorimeterHit>decode(col);
      RawHit_standard_merge_predicate f(decode);
      f.setNeighbourTimeDistance(m_neighbourTimeDistance*200e-9);
      f.setNeighbourStripDistance(m_neighbourStripDistance);
      f.setNeighbourPadDistance(m_neighbourPadDistance);
      f.setPadJunctionByCorner(m_padJunctionByCorner);
      for (int ihit=0; ihit < col->getNumberOfElements(); ++ihit) 
      {
        CalorimeterHit* raw_hit = dynamic_cast<CalorimeterHit*>( col->getElementAt(ihit)) ;
        if (raw_hit != nullptr) 
	      {
	        if(Global::geom->GetDifType(decode(raw_hit)["DIF_Id"])==scintillator||Global::geom->GetDifType(decode(raw_hit)["DIF_Id"])==tcherenkov)
	        {
	          continue;
	        }
	        if(Global::geom->GetDifNbrPlate(decode(raw_hit)["DIF_Id"])==-1)
	        {
	          continue;
	        }
	        HitsByPlanes.push_back(raw_hit);
	      }
	    }
	    LCCollectionVec* col_event1 = new LCCollectionVec(LCIO::CALORIMETERHIT);
      CellIDEncoder<CalorimeterHitImpl> cd1( "I:8,J:7,K:10,DIF_Id:8,Asic_Id:6,Channel:7",col_event1) ;
	    col_event1->setFlag(col_event1->getFlag()|( 1 << LCIO::RCHBIT_LONG));
	    col_event1->setFlag(col_event1->getFlag()|( 1 << LCIO::RCHBIT_TIME));
	    col_event1->setFlag(col_event1->getFlag()|( 1 << LCIO::CHBIT_ID1));
	    col_event1->setFlag(col_event1->getFlag()|( 1 << LCIO::RCHBIT_BARREL));
	    col_event1->setFlag(col_event1->getFlag()|( 1 << LCIO::RCHBIT_ENERGY_ERROR));
	    std::vector<std::vector<CalorimeterHit*>::iterator> clusters;
      SDHCAL::clusterize(HitsByPlanes.begin(),HitsByPlanes.end(),clusters,f);
	    for(unsigned int j=0;j!=clusters.size()-1;++j)
	    {
	      CalorimeterHitImpl* caloHit = new CalorimeterHitImpl();
	      unsigned int i=0;
	      int I=0;
	      int J=0;
	      int K=0;
	      int Thre=0;
	      int pound=0;
	      float time=0;
	      float pos[3];
	      pos[0]=0.0;
	      pos[1]=0.0;
	      pos[2]=0.0;
	      for(std::vector<CalorimeterHit*>::iterator itt=clusters[j];itt!=clusters[j+1];++itt)
	      {
	        time+=float((*itt)->getTime());
	        if((*itt)->getEnergy()>Thre)Thre=(*itt)->getEnergy()>Thre;
	        int I1=decode(*itt)["I"];
	        int J1=decode(*itt)["J"];
	        float X=(*itt)->getPosition()[0];
	        float Y=(*itt)->getPosition()[1];
	        float Z=(*itt)->getPosition()[2];
	        K=decode(*itt)["K"];
	        int pound1=1;
	        i++;
	        if(Global::Histogram!=nullptr)
	        {
	          if(poundGain==true) 
	          {
	            pound1*=1.0*Global::Histogram->getGain(decode(*itt)["I"],decode(*itt)["J"],decode(*itt)["K"]);
	          }
	          if(poundThreshold==true) 
	          {
	            pound1*=1.0*Global::Histogram->getThreshold(decode(*itt)["I"],decode(*itt)["J"],decode(*itt)["K"],(*itt)->getEnergy());
	          }
	        }
	        pos[0]+=X*pound1;
	        pos[1]+=Y*pound1;
	        pos[2]+=Z*pound1;
	        I+=I1*pound1;
	        J+=J1*pound1;
	        pound+=pound1;
	      }
	      hists("Cluster_Size",K,0,0,0,1).Fill(i,1.);
	      I/=(1.0*pound);
	      J/=(1.0*pound);
	      pos[0]/=(1.0*pound);
	      pos[1]/=(1.0*pound);
	      pos[2]/=(1.0*pound);
	      caloHit->setTime(time);
        caloHit->setEnergy(Thre);
        cd1["DIF_Id"]=0;
        cd1["Asic_Id"]=0;
        cd1["Channel"]=0;
        cd1["I"]=ceil(I);
        cd1["J"]=ceil(J);
        cd1["K"]=ceil(K);
        caloHit->setPosition(pos);
        caloHit->setEnergyError(i);
        cd1.setCellID( caloHit ) ;
        col_event1->addElement(caloHit);
	    }    
	    evtP->addCollection(col_event1, "Clusterised");
	    HitsByPlanes.clear();
	    clusters.clear();
	  }
	  catch (DataNotAvailableException &e)
	  {
	    std::cout << "TRIGGER SKIPED ..."<<std::endl;
	    break;
	  } 
  }
}

void Clusterisation::end()
{
  hists.Write();
}
