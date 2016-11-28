#ifndef ProcessorLoader_h
#define ProcessorLoader_h 1
#include "lcio.h"
#include "LCIOSTLTypes.h"
#include <string>
#include <vector>
class ProcessorLoader 
{
  typedef std::vector<void*> LibVec ;
  public:
    ProcessorLoader( lcio::StringVec::const_iterator  first, lcio::StringVec::const_iterator last ) ;
    virtual ~ProcessorLoader() ;
    bool failedLoading() { return _loadError; };
  protected:
    LibVec _libs ;
  private:
    bool _loadError;
};
#endif
