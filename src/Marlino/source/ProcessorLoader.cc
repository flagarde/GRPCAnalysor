#include "ProcessorLoader.h"
#include <dlfcn.h>
#include <iostream>
#include <cstdlib>
#include <set>
#include "Colors.h"

ProcessorLoader::ProcessorLoader(lcio::StringVec::const_iterator first,lcio::StringVec::const_iterator last ) 
{
  _loadError=false;
  lcio::StringVec::const_iterator current = first ;
  std::set<std::string> _checkDuplicateLibs;
  while( current != last )
  {
    std::string libName( *current ) ;
    size_t idx;
    idx = libName.find_last_of("/");
    // the library basename, i.e. /path/to/libBlah.so --> libBlah.so
    std::string libBaseName( libName.substr( idx + 1 ) );
    char *real_path = realpath(libName.c_str(), NULL);
    if( real_path != NULL )
    {
      std::cout <<green<< "<!-- Loading shared library : " << real_path << " ("<< libBaseName << ")-->" <<normal<< std::endl ;
      // use real_path
      free(real_path);
    }
    else
    {
      std::cout <<green<< "<!-- Loading shared library : " << libName << " ("<< libBaseName << ")-->" <<normal<< std::endl ;
    }
    if( _checkDuplicateLibs.find( libBaseName ) == _checkDuplicateLibs.end() )
    {
      _checkDuplicateLibs.insert( libBaseName ) ;
    }
    else
    {
      std::cout <<red<<"<!-- ERROR loading shared library : " << libName<< "    ->    Trying to load DUPLICATE library -->" <<normal<< std::endl ;
      _loadError=true;
    }
    if( ! _loadError )
    {
      void* libPointer  = dlopen( libName.c_str() , RTLD_LAZY | RTLD_GLOBAL) ;
      if( libPointer == 0 )
      {
        std::cout <<red<< "<!-- ERROR loading shared library : " << libName<< "    ->    "   << dlerror() << " -->" <<normal << std::endl ;
        _loadError=true;
      }
      else
      {
        _libs.push_back( libPointer ) ;
      }
    }
    ++current ;
  }
}


ProcessorLoader::~ProcessorLoader() {}
