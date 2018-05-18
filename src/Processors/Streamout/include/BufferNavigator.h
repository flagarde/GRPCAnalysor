#ifndef BUFFER_NAVIGATOR_H
#define BUFFER_NAVIGATOR_H 1
#include <utility>
#include <cstdint>
#include "DIFSlowControl.h"
#include "Buffer.h"

using namespace std;
//class to navigate in the raw data buffer
class SDHCAL_RawBuffer_Navigator
{
public:
    SDHCAL_RawBuffer_Navigator(Buffer b,unsigned int BitsToSkip); //BitsToSkip=92 in 2012, 24 in 2014
    ~SDHCAL_RawBuffer_Navigator()
    {
        if (_theDIFPtr!=NULL) delete _theDIFPtr;
    }
    bool validBuffer()
    {
        return _DIFstartIndex != 0;
    }
    uint32_t getStartOfDIF()
    {
        return _DIFstartIndex;
    }
    uint8_t* getDIFBufferStart()
    {
        return &(_buffer.buffer()[_DIFstartIndex]);
    }
    uint32_t getDIFBufferSize()
    {
        return _buffer.getsize()-_DIFstartIndex;
    }
    Buffer getDIFBuffer()
    {
        return Buffer(getDIFBufferStart(),getDIFBufferSize());
    }
    DIFPtr* getDIFPtr();
    uint32_t getEndOfDIFData()
    {
        return getDIFPtr()->getGetFramePtrReturn()+3;
    }
    uint32_t getSizeAfterDIFPtr()
    {
        return getDIFBufferSize()-getDIFPtr()->getGetFramePtrReturn();
    }
    uint32_t getDIF_CRC();
    bool hasSlowControlData()
    {
        return getDIFBufferStart()[getEndOfDIFData()]==0xb1;
    }
    Buffer getSCBuffer()
    {
        setSCBuffer();
        return _SCbuffer;
    }
    bool badSCData()
    {
        setSCBuffer();
        return _badSCdata;
    }
    Buffer getEndOfAllData();

private:
    void setSCBuffer();
    Buffer _buffer,_SCbuffer;
    uint32_t _DIFstartIndex;
    DIFPtr* _theDIFPtr;
    bool _badSCdata;
};

#endif
