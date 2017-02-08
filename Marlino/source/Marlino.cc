#include "lcio.h"
#include "LCIOSTLTypes.h"
#include "ProcessorMgr.h"
#include "Processor.h"
#include "IO/LCReader.h"
#include "XMLParser.h"
#include "LCTokenizer.h"
#include "Global.h"
#include <sstream>
#include <fstream>
#include <string>
#include <assert.h>
#include <cstring>
#include <algorithm>
#include "ProcessorLoader.h"
#include "Intro.h"
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "ReaderFactory.h"
#include "Histogrammer.h"
using namespace lcio ;
using namespace std ;

std::vector<std::string> SplitFilename (const std::string& str)
{
  std::size_t found = str.find_last_of("/\\");
  std::vector<std::string> a;
  a.push_back(str.substr(0,found)) ;
  a.push_back(str.substr(found+1));
  return a;
}

std::string SplitFilenameSDHCAL (const std::string& str)
{
  std::size_t found = str.find_first_of("_");
  std::string a;
  a=str.substr(found+1);
  std::size_t found2 = a.find_first_of("_");
  a=a.substr(0,found2);
  return a;
}

class RewindDataFilesException : public lcio::Exception
{
  protected:
    RewindDataFilesException() {  /*no_op*/ ; } 
  public: 
    RewindDataFilesException(const Processor* proc)
    {
      message = proc->name()  ;
    }
    virtual ~RewindDataFilesException() throw() { /*no_op*/; } 
}; 

class StopProcessingException : public lcio::Exception
{
  protected:
    StopProcessingException() {  /*no_op*/ ; } 
  public: 
    StopProcessingException(const Processor* proc)
    {
      message = proc->name()  ;
    }
    virtual ~StopProcessingException() throw() { /*no_op*/; } 
}; 

void  createProcessors( const IParser&  parser) ;

int main(int argc, char* argv[] )
{
  Intro();
  try
  {
    StringVec libs ;
    //------ load shared libraries with processors ------
    DIR *dp=nullptr;
    struct dirent *dirp;
    std::string dir="./libMarlino";
    std::string filepath="";
    struct stat filestat;
    dp = opendir( dir.c_str() );
    if (dp == nullptr)
    {
      cout <<red<< "Error(" << errno << ") opening "<<normal << dir << endl;
      return errno;
    }
    while ((dirp = readdir( dp )))
    {
      filepath = dir + "/" + dirp->d_name;
      // If the file is a directory (or is in some way invalid) we'll skip it 
      if (stat( filepath.c_str(), &filestat )) continue;
      if (S_ISDIR( filestat.st_mode ))         continue;
      libs.push_back(filepath);
    }
    closedir( dp );
    if(libs.size()==0) 
    {
      std::cout<<red << std::endl << "<!-- You have no MARLIN_DLL variable in your environment "
            " - so no processors will be loaded. ! --> " <<normal<< std::endl << std::endl ;
    }
    //std::for_each( marlinProcs.begin(), marlinProcs.end(), t ) ;
    ProcessorLoader loader( libs.begin() , libs.end()  ) ;
    if( loader.failedLoading() )
    {
      return(1);
    }
    //------- end processor libs -------------------------
    std::string steeringFileName ="none";
    std::string rootFile ="";
    // read file name from command line
    if( argc > 1 )
    {
      // one argument given: the steering file for normal running :
      steeringFileName = argv[1] ;
      if(argc == 3) rootFile=std::string(argv[2]);
    } 
    else 
    {
      std::cout<<red<<"Not .xml given "<<normal<<std::endl;
      return(1);
    }
    IParser* parser=nullptr ;
    std::string binname = argv[0]  ;
    binname = binname.substr( binname.find_last_of("/") + 1 , binname.size() ) ;
    if( steeringFileName.rfind(".xml") == std::string::npos ||!(  steeringFileName.rfind(".xml")+ strlen(".xml") == steeringFileName.length() ) ) 
    {
      std::cout<<red<<"Not .xml given "<<normal<<std::endl;
      return(1);  
    } 
    else 
    {
      parser = new XMLParser( steeringFileName ) ;
    }
    parser->parse() ;
    Global::parameters = parser->getParameters("Global")  ;
    if( Global::parameters  == nullptr ) 
    {
        std::cout<<red << "  Could not get global parameters from steering file ! " << std::endl  
            << "   The program has to exit - sorry ! " 
            << normal<<std::endl ;
        return(1) ;
    }
    createProcessors( *parser ) ;
    StringVec lcioInputFiles ; 
    if ( (Global::parameters->getStringVals("LCIOInputFiles" , lcioInputFiles ) ).size() == 0 )
    {
        ProcessorMgr::instance()->init() ; 
        ProcessorMgr::instance()->end() ; 
    } 
    else 
    { 
      std::map<std::string,std::set<std::string>>files;
      std::vector<std::vector<std::string>> files_well_ordered;
      for(unsigned int i =0;i!=lcioInputFiles.size();++i)
      {
        std::string path=SplitFilename(lcioInputFiles[i])[0];
        std::string name=SplitFilename(lcioInputFiles[i])[1];
        std::string number = SplitFilenameSDHCAL(name);
        files[number].insert(lcioInputFiles[i]);
      }
      std::set<std::string> numbers;
      for(unsigned int i =0;i!=lcioInputFiles.size();++i)
      {
        std::string name=SplitFilename(lcioInputFiles[i])[1];
        std::string number = SplitFilenameSDHCAL(name);
        if(numbers.find(number)==numbers.end())
        {
          numbers.insert(number);
          std::vector<std::string> a;
          for(std::set<std::string>::iterator it=files[number].begin();it!=files[number].end();++it)
          {
            a.push_back(*it);
          }
          files_well_ordered.push_back(a);
        }
      }
      int maxRecord = Global::parameters->getIntVal("MaxRecordNumber") ;
      if(rootFile==""&&Global::parameters->getStringVal("RootFileName")!="")rootFile=Global::parameters->getIntVal("RootFileName");
      else if(rootFile==""&&Global::parameters->getStringVal("RootFileName")=="") rootFile="Default.root";
      int skipNEvents = Global::parameters->getIntVal("SkipNEvents");
      bool modify = ( Global::parameters->getStringVal("AllowToModifyEvent") == "true" ) ;
      std::string reader_type = Global::parameters->getStringVal("ReaderType");
      std::string FileNameGeometry="";
      FileNameGeometry = Global::parameters->getStringVal("FileNameGeometry");
      std::string FileNameElog="";
      FileNameElog = Global::parameters->getStringVal("FileNameElog");
      if( modify ) 
      {
	          std::cout<<yellow  << " ******************************************************************************* \n" 
				    << " *    AllowToModifyEvent is set to 'true'                                      * \n"
				    << " *    => all processors can modify the input event in processEvent() !!        * \n"
				    << " *        consider setting this flag to 'false'                                * \n"
				    << " *        unless you really need it...                                         * \n"
				    << " *    - if you need a processor that modifies the input event                  * \n"
				    << " *      please implement the EventModifier interface and use the modifyEvent() * \n"
				    << " *      method for this                                                        * \n"
 				    << " ******************************************************************************* \n" 
				    <<normal<< std::endl ;
	    }
      // create lcio reader 
      ReaderFactory* readerFactory = new ReaderFactory;
      Reader* reader = readerFactory->CreateReader(reader_type);
      if(reader&&FileNameGeometry!=""&&readerFactory!=nullptr) 
      {
        Global::geom= new Geometry;
        reader->Read(FileNameGeometry,*Global::geom);
        Global::geom->PrintGeom();
        delete reader;
      }
      Global::out = new  OutFileRoot;
      Global::out->setOutputFile(rootFile);
      LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;
      StringVec readColNames ; 
	    if( (Global::parameters->getStringVals("LCIOReadCollectionNames" , readColNames ) ).size() != 0 )
	    {
	      std::cout<<yellow  << " *********** Parameter LCIOReadCollectionNames given - will only read the following collections: **** " 
				<<normal<< std::endl ;
        for( unsigned i=0,N=readColNames.size() ; i<N ; ++i ) std::cout <<yellow << "     " << readColNames[i]<<normal<< std::endl ;
	      std::cout<<yellow  << " *************************************************************************************************** "         <<normal<<std::endl ;
	      lcReader->setReadCollectionNames( readColNames ) ;
	    }  
      lcReader->registerLCRunListener( ProcessorMgr::instance() ) ; 
      lcReader->registerLCEventListener( ProcessorMgr::instance() ) ;
      for(unsigned int i =0;i!=files_well_ordered.size();++i)
      {
        LCReader* lcReadercounter = LCFactory::getInstance()->createLCReader() ;
        lcReadercounter->open( files_well_ordered[i] ) ;
        LCEvent *evt(nullptr);
        do 
        {
          Global::totalevent++;
          evt = lcReadercounter->readNextEvent();
        } 
        while (evt != nullptr);
        lcReadercounter->close() ;
        delete lcReadercounter;
        delete evt;
        std::string name=SplitFilename(files_well_ordered[i][0])[1];
        std::string number = SplitFilenameSDHCAL(name);
        Global::number=stoi(number);
        std::string RunName="Run_"+number+"/";
        Global::out->setRunName(RunName);
        if(FileNameElog!="") 
        {
          Global::conf = new ConfigInfos;
          Reader* elogreader=readerFactory->CreateReader("XMLReaderElog");
          elogreader->Read(FileNameElog,Global::conf,Global::number);
          Histogrammer(Global::conf,Global::out,Global::geom);
          delete elogreader;
        }
        //Global::LCIOFiles=&files_well_ordered[i];
        ProcessorMgr::instance()->init() ; 
        bool rewind = true ;
        while( rewind ) 
        {    
          rewind = false ;
          // process the data
          lcReader->open( files_well_ordered[i]  ) ; 
          if( skipNEvents > 0 )
          {
            std::cout<<yellow << " --- Marlin.cc - will skip first " << skipNEvents << " event(s)" <<normal << std::endl << std::endl ;
            lcReader->skipNEvents(  skipNEvents ) ;
          }
          try
          { 
            if( maxRecord > 0 )
            {
              try
              {
                lcReader->readStream( maxRecord ) ;
              }
              catch( lcio::EndOfDataException& e)
              {
                std::cout << e.what() << std::endl ;
              }
            } 
            else 
            {
              lcReader->readStream() ;
            }
          } 
          catch( StopProcessingException &e) 
          {
            std::cout  << std::endl
            <<red<< " **********************************************************" << std::endl
            << " *                                                        *" << std::endl
            << " *   Stop of EventProcessiong requested by processor :    *" << std::endl
            << " *                  "  << e.what()                           << std::endl
            << " *     will call end() method of all processors !         *" << std::endl
            << " *                                                        *" << std::endl
            << " **********************************************************" << std::endl
            << normal<<std::endl ;
          } 
          catch( RewindDataFilesException &e) 
          {
            rewind = true ;
            std::cout  << std::endl
            <<red << " **********************************************************" << std::endl
            << " *                                                        *" << std::endl
            << " *   Rewind data files requested by processor :           *" << std::endl
            << " *                  "  << e.what()                           << std::endl
            << " *     will rewind to beginning !                         *" << std::endl
            << " *                                                        *" << std::endl
            << " **********************************************************" << std::endl
            <<normal<< std::endl ;
          }
          lcReader->close() ;
        }// end rewind
        ProcessorMgr::instance()->end() ;
        Global::totalevent=0;
      }  
     delete lcReader ;
     delete readerFactory;
    }
  } 
  catch( std::exception& e) 
  {
    std::cerr <<red<< " ***********************************************\n" 
	  << " A runtime error occured - (uncaught exception):\n" 
	  << "      " <<    e.what() << "\n"
	  << " Marlin will have to be terminated, sorry.\n"  
	  << " ***********************************************\n" 
	  << normal<<std:: endl ;
    return 1 ;
    delete Global::geom;
    delete Global::conf;
    delete Global::out;
  }
  delete Global::geom;
  delete Global::conf;
  delete Global::out;
  return 0 ;
}

void createProcessors( const IParser&  parser) 
{
  StringVec activeProcessors ;
  Global::parameters->getStringVals("ActiveProcessors" , activeProcessors ) ;
  StringVec procConds ;
  Global::parameters->getStringVals("ProcessorConditions" , procConds ) ;
  bool useConditions = ( activeProcessors.size() == procConds.size() ) ;
  for(unsigned int i=0 ; i<  activeProcessors.size() ; i++ ) 
  {
    StringParameters* p = parser.getParameters( activeProcessors[i] )  ;
    if( p!=nullptr )
    {
      std::string type = p->getStringVal("ProcessorType") ;
      if( useConditions ) ProcessorMgr::instance()->addActiveProcessor( type , activeProcessors[i] , p , procConds[i] )  ;
      else ProcessorMgr::instance()->addActiveProcessor( type , activeProcessors[i] , p )  ;
    } 
    else
    {
      std::stringstream sstr ;
      sstr<<red << "Undefined processor : " << activeProcessors[i] <<normal<< std::endl ;
      std::cout  << sstr.str() ;	
      throw Exception( sstr.str() );
    }
  }
}
