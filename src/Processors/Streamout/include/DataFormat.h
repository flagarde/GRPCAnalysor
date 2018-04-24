#pragma once
#include "UTIL/CellIDEncoder.h"
#include "IMPL/RawCalorimeterHitImpl.h"
#include "IMPL/LCCollectionVec.h"
#include "LMGenericObject.h"
//#include "DIFUnpacker.h"


//#include <IMPL/RawCalorimeterHitImpl.h>
//#include <EVENT/LCGenericObject.h>
//#include <IMPL/LCGenericObjectImpl.h>

//#include <string>
//#include <algorithm>
//#include <map>
//#include <iostream>
//#include <string>
//#include <cstdlib>
//#include <cmath>
//#include <cstdint>
//#include <EVENT/LCCollection.h>
//#include <EVENT/LCGenericObject.h>
//#include <IMPL/LCGenericObjectImpl.h>
//#include <UTIL/LCTOOLS.h>

//#include "IMPL/LCFlagImpl.h"
//#include "IMPL/LCParametersImpl.h"

//#include <bitset>
//#include "lcio.h"

//#include "IMPL/LCEventImpl.h"
//#include "IMPL/CalorimeterHitImpl.h"
//#include <IMPL/LCRunHeaderImpl.h>

//#include <sstream>
//#include <set>
//#include <limits>

class DataFormat
{
public:
        virtual void SetCellIDEncoder(UTIL::CellIDEncoder<IMPL::RawCalorimeterHitImpl>& cd)
        {
           _cd=&cd;
        }
        virtual void SetLCCollectionVec(IMPL::LCCollectionVec* RawVec)
        {
          _RawVec=RawVec;
        }
        DataFormat()
        {
            RawBuffer=nullptr;
            _cd=nullptr;
        }
        virtual ~DataFormat()
        {
            
        }
        virtual bool SetRawBuffer(EVENT::LCObject * obj)
        {
            RawBuffer=(LMGeneric *)(obj);
            if (RawBuffer==nullptr) 
            {
                _nWrongObj++;
                return true;
            }
            else 
            {
                _nProcessedObject++;
                return false;
            }
        }
        unsigned int GetNbWrongObj()
        {
          return _nWrongObj;
        }
        unsigned int GetNbrProcessedObject()
        {
          return _nProcessedObject;
        }
        void SetBitToSkip(int BitsToSkip)
        {
           _BitsToSkip=(unsigned int)(BitsToSkip);
        }
        void SetVirerFullAsics(bool virer_full_asic)
        {
            _virer_full_asic=virer_full_asic;
        }
        virtual void Parse()= 0;
protected :
        LMGeneric* RawBuffer{nullptr};
        static unsigned int _nWrongObj;
        static unsigned int _nProcessedObject;
        unsigned int _BitsToSkip{24};
        bool _virer_full_asic{false};
        UTIL::CellIDEncoder<IMPL::RawCalorimeterHitImpl>* _cd;
        IMPL::LCCollectionVec* _RawVec;
};


