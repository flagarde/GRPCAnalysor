#include "AEGIS.h"
#include <iostream>
#include <string>
#include "Global.h"
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
using namespace lcio ;


AEGIS aAEGIS;

AEGIS::AEGIS():Processor("AEGIS")
{
  _hcalCollections={"XYZFilled"};
  registerInputCollections( LCIO::CALORIMETERHIT,"HCALCollections","HCAL Collection Names",_hcalCollections,_hcalCollections);
} // end constructor

void AEGIS::processRunHeader( LCRunHeader* run){}

void AEGIS::init()
{ 
  printParameters();
  TXTFile.open(std::to_string(Global::number)+".txt");
} // end init method

void AEGIS::processEvent( LCEvent * evtP )
{
  std::cout<<"yyyyy"<<std::endl;
  if (evtP!= nullptr )
  {
    try 
	    {
    for(unsigned int i=0; i< _hcalCollections.size(); i++) 
    {
              std::cout<<"hjjjjjjjjjjj"<<std::endl;
std::cout<<_hcalCollections[i]<<std::endl;
	      LCCollection* col = evtP ->getCollection(_hcalCollections[i].c_str());
          CellIDDecoder<CalorimeterHitImpl>decode(col);
          for (int ihit=0; ihit < col->getNumberOfElements(); ++ihit) 
          {
            std::cout<<"hh"<<std::endl;
            CalorimeterHitImpl *raw = dynamic_cast<CalorimeterHitImpl*>( col->getElementAt(ihit)) ;
           TXTFile<<decode(raw)["DIF_Id"]<<" "<<decode(raw)["I"]<<" "<<decode(raw)["J"]<<" "<<decode(raw)["K"]<<" "<<raw->getPosition()[0]<<" "<<raw->getPosition()[1]
            <<" "<<raw->getPosition()[2]<<" "<<raw->getTime()<<" "<<raw->getEnergy()<<std::endl;
    } //end  for(unsigned int i=0; i< _hcalCollections.size(); i++) 
  }
  }
  catch (DataNotAvailableException &e)
	    {
	      std::cout << "TRIGGER SKIPED !!!!..."<<std::endl;
	    }
 }
} //end processEvent

void AEGIS::end()
{
    TXTFile.close();
}
