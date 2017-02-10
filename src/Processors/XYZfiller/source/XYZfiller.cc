#include "XYZfiller.h"
#include "Global.h"
#include <iostream>
#include <string>
#include "Processor.h"
#include "IMPL/LCCollectionVec.h"
#include "UTIL/CellIDEncoder.h"
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
#define degtorad 0.0174532925
#define size_pad 10.4125
#define size_strip 2.5
using namespace lcio ;

void XYZfiller::FillXYZ(CalorimeterHitImpl* ev, LCCollectionVec* col,CellIDEncoder<CalorimeterHitImpl>& cd, CellIDDecoder<CalorimeterHitImpl>& deco)
{
  CalorimeterHitImpl* caloHit = new CalorimeterHitImpl();
  caloHit->setTime(float((*ev).getTime()));
  caloHit->setEnergy(float((*ev).getEnergy()));
  cd["DIF_Id"]=int(deco(ev)["DIF_Id"]);
  cd["Asic_Id"]=int(deco(ev)["Asic_Id"]);
  cd["Channel"]=int(deco(ev)["Channel"]);
  cd["I"]=int(deco(ev)["I"]);
  cd["J"]=int(deco(ev)["J"]);
  cd["K"]=int(deco(ev)["K"]);
  double ca=SinCos[int(cd["DIF_Id"])][0];
	double sa=SinCos[int(cd["DIF_Id"])][1];
  double cb=SinCos[int(cd["DIF_Id"])][2];
	double sb=SinCos[int(cd["DIF_Id"])][3];
  double cg=SinCos[int(cd["DIF_Id"])][4];
	double sg=SinCos[int(cd["DIF_Id"])][5];
	unsigned int NbrPlate =Global::geom->GetDifNbrPlate(deco(ev)["DIF_Id"])-1;
  double Z= Global::geom->GetPlatePositionZ(NbrPlate);
	float pos[3];
	if(Global::geom->GetDifType(cd["DIF_Id"])==pad) 
  {
    pos[0] = cg*cb*cd["I"]*size_pad+(-sg*ca+cg*sb*sa)*cd["J"]*size_pad+(sg*sa+cg*sb*ca)*Z+Global::geom->GetPlatePositionX(NbrPlate);
    pos[1] = sg*cb*cd["I"]*size_pad+(cg*ca+sg*sb*sa)*cd["J"]*size_pad+(-cg*sa+sg*sb*ca)*Z+Global::geom->GetPlatePositionY(NbrPlate);
    pos[2] = -sb*cd["I"]*size_pad+cb*sa*cd["J"]*size_pad+cb*ca*Z;
  }
  if(Global::geom->GetDifType(cd["DIF_Id"])==positional) 
  {
    if(cd["Asic_Id"]%2==1) pos[0]=cg*cb*cd["I"]*size_strip+(-sg*ca+cg*sb*sa)*cd["J"]*size_strip+(sg*sa+cg*sb*ca)*Z+Global::geom->GetPlatePositionX(NbrPlate)+1;
    else pos[0] = cg*cb*cd["I"]*size_strip+(-sg*ca+cg*sb*sa)*cd["J"]*size_strip+(sg*sa+cg*sb*ca)*Z+Global::geom->GetPlatePositionX(NbrPlate);
    pos[1] = sg*cb*cd["I"]*size_strip+(cg*ca+sg*sb*sa)*cd["J"]*size_strip+(-cg*sa+sg*sb*ca)*Z+Global::geom->GetPlatePositionY(NbrPlate);
    pos[2] = -sb*cd["I"]*size_strip+cb*sa*cd["J"]*size_strip+cb*ca*Z;
  }
  caloHit->setPosition(pos);
  cd.setCellID( caloHit ) ;
  col->addElement(caloHit);
}

XYZfiller aXYZfiller;

XYZfiller::XYZfiller():Processor("XYZfiller")
{
  _hcalCollections={"IJKFilled"};
  registerInputCollections( LCIO::CALORIMETERHIT,"HCALCollections","HCAL Collection Names",_hcalCollections,_hcalCollections);
} // end constructor

void XYZfiller::processRunHeader( LCRunHeader* run){}

void XYZfiller::init()
{ 
  printParameters();
  std::map<int, Dif >Difs=Global::geom->GetDifs();;
  for(std::map<int, Dif >::iterator it=Difs.begin(); it!=Difs.end(); ++it) 
	{
    if(Global::geom->GetDifType(it->first)!=temporal&&Global::geom->GetDifType(it->first)!=tcherenkov&&Global::geom->GetDifType(it->first)!=scintillator) 
	  {
	    SinCos[it->first]=std::vector<double>{
	                                         cos(Global::geom->GetDifAlpha(it->first)*degtorad),
	                                         sin(Global::geom->GetDifAlpha(it->first)*degtorad),
	                                         cos(Global::geom->GetDifBeta(it->first)*degtorad),
	                                         sin(Global::geom->GetDifBeta(it->first)*degtorad),
	                                         cos(Global::geom->GetDifGamma(it->first)*degtorad),
	                                         sin(Global::geom->GetDifGamma(it->first)*degtorad)
	                                         };
    }
    else SinCos[it->first]={0,0,0,0,0,0};
  }
} // end init method

void XYZfiller::processEvent( LCEvent * evtP )
{
  if (evtP!= nullptr )
  {
    for(unsigned int i=0; i< _hcalCollections.size(); i++) 
    {
      try 
	    {
	      LCCollection* col = evtP ->getCollection(_hcalCollections[i].c_str());
        CellIDDecoder<CalorimeterHitImpl>decode(col);
        LCCollectionVec* col_event1 = new LCCollectionVec(LCIO::CALORIMETERHIT);
        CellIDEncoder<CalorimeterHitImpl> cd1( "I:8,J:7,K:10,DIF_Id:8,Asic_Id:6,Channel:7",col_event1) ;
	      col_event1->setFlag(col_event1->getFlag()|( 1 << LCIO::RCHBIT_LONG));
	      col_event1->setFlag(col_event1->getFlag()|( 1 << LCIO::RCHBIT_TIME));
	      col_event1->setFlag(col_event1->getFlag()|( 1 << LCIO::CHBIT_ID1));
	      col_event1->setFlag(col_event1->getFlag()|( 1 << LCIO::RCHBIT_ENERGY_ERROR));
        for (int ihit=0; ihit < col->getNumberOfElements(); ++ihit) 
        {
	        CalorimeterHitImpl *raw = dynamic_cast<CalorimeterHitImpl*>( col->getElementAt(ihit)) ;
	        if(Global::geom->GetDifNbrPlate(decode(raw)["DIF_Id"])!=-1) FillXYZ(raw,col_event1,cd1,decode);
	      }
	      evtP->addCollection(col_event1, "XYZFilled");
	    }// end try block
      catch (DataNotAvailableException &e)
	    {
	      std::cout << "TRIGGER SKIPED !!!!..."<<std::endl;
	      break;
	    }
    } //end  for(unsigned int i=0; i< _hcalCollections.size(); i++) 
  }
} //end processEvent

void XYZfiller::end(){}
