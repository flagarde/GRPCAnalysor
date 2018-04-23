#include "Buffer.h"
#include <iostream>
#include <cstdint>
void Buffer::printBuffer(unsigned int start, unsigned int stop,std::ostream& flux)
{
    flux << std::hex;
    for (unsigned int k=start; k<stop; k++) flux << (unsigned int)(first[k]) << " - ";
    flux << std::dec <<  std::endl;
}
