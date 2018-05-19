#pragma once
#include "UTIL/CellIDEncoder.h"
#include "IMPL/RawCalorimeterHitImpl.h"
#include "IMPL/LCCollectionVec.h"
#include "LMGenericObject.h"
#include "Colors.h"
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
        virtual bool SetRawBuffer(EVENT::LCObject* obj)
        {
            RawBuffer=(LMGeneric *)(obj);
            if (RawBuffer==nullptr) 
            {
                _nWrongObj++;
                //std::cout<<red<<_nWrongObj<<normal<<std::endl;
                return true;
            }
            else 
            {
                _nProcessedObject++;
                //std::cout<<green<<_nProcessedObject<<normal<<std::endl;
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
        virtual int Parse()
        {
            return -1;
        }
protected :
        LMGeneric* RawBuffer{nullptr};
        static unsigned int _nWrongObj;
        static unsigned int _nProcessedObject;
        unsigned int _BitsToSkip{24};
        bool _virer_full_asic{false};
        UTIL::CellIDEncoder<IMPL::RawCalorimeterHitImpl>* _cd;
        IMPL::LCCollectionVec* _RawVec;
};


