#pragma once 
#include "SDHCAL_DataFormat.h"
#include "LM_DataFormat.h"
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
class ParseDataFormat
{
    private:
        DataFormat* m_data;
        std::map<std::string,DataFormat*> DataFormatNames={{"LM",new LM_DataFormat()},{"SDHCAL",new SDHCAL_DataFormat}};
    public:
        ParseDataFormat(){}
        void set_DataFormat(std::string DataFormatName,IMPL::LCCollectionVec* RawVec,UTIL::CellIDEncoder<IMPL::RawCalorimeterHitImpl>& cd)
        {
            if(DataFormatNames.find(DataFormatName)==DataFormatNames.end())
            {
                std::cout<<"This Data Format ("+DataFormatName+" is not yet implemented"<<std::endl;
            }
            else
            {
                m_data=DataFormatNames.find(DataFormatName)->second;
                m_data->SetCellIDEncoder(cd);
                m_data->SetLCCollectionVec(RawVec);
            }
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

