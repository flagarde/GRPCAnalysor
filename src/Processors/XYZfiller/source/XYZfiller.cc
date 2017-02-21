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
#include "ConstructConverters.h"
using namespace lcio ;

void XYZfiller::FillXYZ(CalorimeterHitImpl* ev, LCCollectionVec* col,CellIDEncoder<CalorimeterHitImpl>& cd, CellIDDecoder<CalorimeterHitImpl>& deco)
{
  if(Global::Global::geom->GetDifNbrPlate(deco(ev)["DIF_Id"])!=-1||_SupressHitsOfDifsNotInXML==false) 
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
  	converter->setType(int(deco(ev)["DIF_Id"]));
	  float pos[3]={converter->IJKToXYZ(cd["I"],cd["J"],cd["K"])[0],converter->IJKToXYZ(cd["I"],cd["J"],cd["K"])[1],converter->IJKToXYZ(cd["I"],cd["J"],cd["K"])[2]};
    caloHit->setPosition(pos);
    cd.setCellID( caloHit ) ;
    col->addElement(caloHit);
  }
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
  converter=new ConstructConverters(Global::geom);
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
	        if(Global::Global::geom->GetDifNbrPlate(decode(raw)["DIF_Id"])==-1) 
	        {
	          if(Warningg[decode(raw)["DIF_Id"]]!=true) 
		        {
		          Warningg[decode(raw)["DIF_Id"]]=true;
		          if(_SupressHitsOfDifsNotInXML==false)
		          {
		            std::cout<<red<<"Please add DIF "<<decode(raw)["DIF_Id"]<<" to your geometry file; Dif considered as pads type"<<normal<<std::endl; 
		          }
		          else std::cout<<red<<"Please add DIF "<<decode(raw)["DIF_Id"]<<" to your geometry file; I'm skipping its data"<<normal<<std::endl; 
		        }
	        }
          FillXYZ(raw,col_event1,cd1,decode);
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

void XYZfiller::end()
{
  for(std::map<int,bool>::iterator it=Warningg.begin(); it!=Warningg.end(); it++) 
  {
    if(_SupressHitsOfDifsNotInXML==true)
		{
		  std::cout<<red<<"REMINDER::Data from Dif "<<it->first<<" are skipped !"<<normal<<std::endl;
		}
    else std::cout<<red<<"REMINDER::Data from Dif "<<it->first<<" are considered as pads type"<<normal<<std::endl;
  }
}
