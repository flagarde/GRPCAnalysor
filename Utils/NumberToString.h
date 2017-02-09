#ifndef NumberToString
#define NumberToString
#include <string>
#include <sstream>

template <typename T>
std::string NumberToString(T pNumber)
{
 std::ostringstream oOStrStream;
 oOStrStream << pNumber;
 return oOStrStream.str();
}
#endif 
