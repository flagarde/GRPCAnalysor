#pragma once 
#include "SDHCAL_DataFormat.h"
#include "LM_DataFormat.h"
#include "Colors.h"
//#include "UTIL/CellIDEncoder.h"
//#include "LMGenericObject.h"
//#include <IMPL/RawCalorimeterHitImpl.h>
//#include <EVENT/LCGenericObject.h>
//#include <IMPL/LCGenericObjectImpl.h>
//#include <string>
//#include <algorithm>
//#include <map>
//#include <iostream>
//#include <cstdlib>
//#include <cmath>
//#include <cstdint>
//#include <EVENT/LCCollection.h>
//#include <UTIL/LCTOOLS.h>
//#include "IMPL/LCCollectionVec.h"
//#include "UTIL/LCTOOLS.h"
//#include <sstream>
//#include <set>
#include <iostream>
class ParseDataFormat
{
    private:
        DataFormat* m_data;
        std::map<std::string,DataFormat*> DataFormatNames={{"LM",new LM_DataFormat()},{"SDHCAL",new SDHCAL_DataFormat()}};
    public:
        ParseDataFormat(){}
        bool set_DataFormat(std::string DataFormatName)
        {
            if(DataFormatNames.find(DataFormatName)==DataFormatNames.end())
            {
                std::cout<<red<<"The Data Format \""+DataFormatName+"\" is not yet implemented"<<normal<<std::endl;
                return false;
            }
            else
            {
                m_data=DataFormatNames.find(DataFormatName)->second;
                return true;
            }
        }
        void SetCellIDEncoder(UTIL::CellIDEncoder<IMPL::RawCalorimeterHitImpl>& cd)
        {
            m_data->SetCellIDEncoder(cd);
        }
        void SetLCCollectionVec(IMPL::LCCollectionVec* RawVec)
        {
            m_data->SetLCCollectionVec(RawVec);
        }
        ~ParseDataFormat()
        {
          delete m_data;
          for(std::map<std::string,DataFormat*>::iterator it=DataFormatNames.begin();it!=DataFormatNames.end();++it) delete it->second;
          DataFormatNames.clear();
        }
        void Parse() const 
        {
            m_data->Parse();
        }
        void SetBitToSkip(int BitsToSkip)
        {
           m_data->SetBitToSkip(BitsToSkip);
        }
        void SetVirerFullAsics(bool virer_full_asic)
        {
            m_data->SetVirerFullAsics(virer_full_asic);
        }
        bool SetRawBuffer(EVENT::LCObject * obj)
        {
            return m_data->SetRawBuffer(obj);
        }
};

