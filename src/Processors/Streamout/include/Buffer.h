#pragma once
#include <cstdint>
#include <utility>
#include <iostream>
using namespace std;
class Buffer : public std::pair<uint8_t*, uint32_t>
{
    public:
    Buffer(uint8_t* b, uint32_t i) : pair<uint8_t*, uint32_t>(b,i){}
    uint8_t* buffer()
    {
        return first;
    }
    uint8_t* endOfBuffer()
    {
        return first+second;
    }
    uint32_t getsize()
    {
        return second;
    }
    void printBuffer(uint32_t start, uint32_t stop,std::ostream& flux=std::cout);
    void printBuffer(uint32_t start=0,std::ostream& flux=std::cout)
    {
        printBuffer(start,getsize());
    }
};
