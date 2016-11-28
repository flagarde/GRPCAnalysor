#include "XMLParser.h"
#include "tinyxml.h"
#include <algorithm>
#include <sstream>
#include <set>
#include <memory>
#include "Colors.h"
/**ParseException used for parse errors, e.g. when reading the steering file.
* 
* @author gaede
* @version $Id: Exceptions.h,v 1.5 2007-02-02 17:15:25 gaede Exp $
*/
class ParseException : public lcio::Exception
{
  protected:
    ParseException() {  /*no_op*/ ; } 
  public: 
    virtual ~ParseException() throw() { /*no_op*/; } 
    ParseException( std::string text )
    {
      message = red+"marlin::ParseException: " + text+normal ;
    }
}; 

// open steering file with processor names 
XMLParser::XMLParser( const std::string&  fileName) :_current(nullptr) , _doc(nullptr), _fileName( fileName ) {}

XMLParser::~XMLParser(){}

void XMLParser::parse()
{
  _doc = new TiXmlDocument ;
  bool loadOkay = _doc->LoadFile(_fileName  ) ;
  if( !loadOkay ) 
  {
    std::stringstream str ;
    str  << "XMLParser::parse error in file [" << _fileName 
         << ", row: " << _doc->ErrorRow() << ", col: " << _doc->ErrorCol() << "] : "
         << _doc->ErrorDesc() ;
    throw ParseException( str.str() ) ;
  }
  TiXmlElement* root = _doc->RootElement();
  if( root==nullptr || strcmp(root->Value(),"marlin") != 0 )
  {
    throw ParseException(std::string( "XMLParser::parse : no root tag <marlin>...</marlin> found in  ") + _fileName  ) ;
  }
  TiXmlNode* section = nullptr ;
  StringParameters*  globalParameters = new StringParameters() ;
  _map[ "Global" ] = globalParameters ;
  section = root->FirstChild("global")  ;
  if( section != nullptr  )
  {
    _current =  _map[ "Global" ] ;
    parametersFromNode( section ) ;
  }  
  else 
  {
    throw ParseException(std::string( "XMLParser::parse : no <global/> section found in  ") + _fileName  ) ;
  }
  std::vector<std::string> activeProcs ;
  activeProcs.push_back("ActiveProcessors") ;
  std::vector<std::string> procConds ;
  procConds.push_back("ProcessorConditions") ;
  // variable used to check for duplicate processor entries in the execute section
  std::set<std::string> procList ;
  section = root->FirstChild("execute")  ;
  if( section != nullptr  )
  {
    // preprocess groups: replace with <processor/> tags
    replacegroups(  section )  ;
    // process processor conditions:
    processconditions( section , "" ) ;
    TiXmlNode* proc = nullptr ;
    while( ( proc = section->IterateChildren( "processor", proc ) )  != nullptr  )
    {
      std::string procName( getAttribute( proc, "name") );
      // exit if processor defined more than once in the execute section
      if( procList.find( procName ) != procList.end() )
      {
        throw ParseException(std::string( "XMLParser::parse : "+ procName +" defined more than once in <execute> section ") );
      }
      procList.insert( procName ) ;
      activeProcs.push_back( procName ) ;
      std::string condition(  getAttribute( proc,"condition")  ) ;
      if( condition.size() == 0 ) condition += "true" ;
      procConds.push_back( condition  ) ;
    }
  } 
  else 
  {
    throw ParseException(std::string( "XMLParser::parse : no <execute/> section found in  ") + _fileName  ) ;
  }
  _current->add( activeProcs ) ;
  _current->add( procConds ) ;

  // preprocess groups: ---------------------------------------------------------------------------------
  // simply copy all group parameters to the processors
  // and then copy the processors to the root node <Marlin>
  while( (section = root->IterateChildren( "group", section ) )  != nullptr  )
  {
    std::vector<TiXmlNode*> groupParams ;
    TiXmlNode* param = nullptr ;
    while( ( param = section->IterateChildren( "parameter" , param ) )  != nullptr  )groupParams.push_back( param->Clone() ) ;
    TiXmlNode* proc = nullptr ;
    while( ( proc = section->IterateChildren( "processor" , proc ) )  != nullptr  )
    {
      for( std::vector<TiXmlNode*>::iterator it = groupParams.begin() ; it != groupParams.end() ; it++) proc->InsertEndChild( **it ) ;
      std::unique_ptr<TiXmlNode> clone (proc->Clone());
      root->InsertBeforeChild( section , *clone  ) ;   // FIXME: memory mngmt. ?
    }
    root->RemoveChild( section ) ;
    for( std::vector<TiXmlNode*>::iterator it = groupParams.begin() ; it != groupParams.end() ; it++)delete *it ;
  }
  // process processor sections -------------------------------------------------------------------------
  std::vector<std::string> availableProcs ;
  availableProcs.push_back("AvailableProcessors") ;
  // count processors with collection type information in order to generate warning
  // about old files or missing type info
  std::pair<unsigned,unsigned> typeCount ;
  unsigned procCount(0) ; 
  unsigned typedProcCount(0) ;
  // use this variable to check for duplicate processor definitions
  procList.clear();
  while( (section = root->IterateChildren("processor",  section ) )  != 0  )
  {
    _current = new StringParameters() ;
    std::string name( getAttribute( section, "name") ) ;
    _map[ name  ] =  _current ;
    // exit if processor defined more than once in the execute section
    if( procList.find( name ) != procList.end() )
    {
      throw ParseException(std::string( "XMLParser::parse : "+ name +" defined more than once in the steering file ") );
    }
    procList.insert( name ) ;
    availableProcs.push_back( name ) ; 
    // add ProcessorType to processor parameters
    std::vector<std::string> procType(2)  ;
    procType[0] = "ProcessorType" ;
    procType[1] =   getAttribute( section, "type")  ;
    _current->add( procType ) ;
    std::pair<unsigned,unsigned> currentCount( typeCount ) ;
    parametersFromNode( section , &typeCount ) ;
    if( typeCount.first > currentCount.first || typeCount.second > currentCount.second )
    {
      ++typedProcCount ; // at least one type info attribute found in processor
    }
    ++procCount ;
  }
  globalParameters->add( availableProcs )  ;
}

const char* XMLParser::getAttribute( TiXmlNode* node , const std::string& name )
{
  TiXmlElement* el = node->ToElement() ;
  if( el == nullptr ) throw ParseException("XMLParser::getAttribute not an XMLElement " ) ;
  const char* at = el->Attribute( name.c_str() )  ;
  if( at == nullptr )
  {
    std::stringstream str ;
    str  << "XMLParser::getAttribute missing attribute \"" << name 
         << "\" in element <" << el->Value() << "/> in file " << _fileName  ;
    throw ParseException( str.str() ) ;
  }
  return at ;
}  

void XMLParser::parametersFromNode(TiXmlNode* section, std::pair<unsigned,unsigned>*typeCount) 
{ 
  TiXmlNode* par = nullptr ;
  std::string index1, index2;
	index1 = section->Value() ;
	if( index1.compare( "processor" ) == 0 )
	{
	  index1 = getAttribute( section, "name") ;
	}
	while( ( par = section->IterateChildren( "parameter", par ) )  != nullptr  )
	{
    index2 = par->ToElement()->Attribute("name") ;
    std::vector<std::string> tokens ;
    std::string name( getAttribute( par, "name" )  ) ;
    tokens.push_back(  name ) ; // first token is parameter name 
    LCTokenizer t( tokens ,' ') ;
    std::string inputLine("") ;
    try
    {  
      inputLine = getAttribute( par , "value" )  ; 
    }      
    catch( ParseException ) 
    {
      if( par->FirstChild() ) inputLine =  par->FirstChild()->Value() ;
    }
    std::for_each( inputLine.begin(),inputLine.end(), t ) ; 
    _current->add( tokens ) ;
    //--------------- check for lcio input/output type attributes -----------
    std::vector<std::string> lcioInTypes ;
    std::vector<std::string> lcioOutTypes ;
    lcioInTypes.push_back( "_marlin.lcioInType" ) ;
    lcioOutTypes.push_back( "_marlin.lcioOutType" ) ;
    std::string colType("")  ;
    try
    {  
      colType = getAttribute( par , "lcioInType" )  ; 
      ++typeCount->first ; // count type description to identify old files w/o type description
      lcioInTypes.push_back( name ) ; 
      lcioInTypes.push_back( colType ) ; 
    }      
    catch( ParseException ) { }
    try
    {  
      colType = getAttribute( par , "lcioOutType" )  ; 
      ++typeCount->second ; // count type description to identify old files w/o type description
      lcioOutTypes.push_back( name ) ; 
      lcioOutTypes.push_back( colType ) ; 
    }      
    catch( ParseException ) { }
    _current->add( lcioInTypes  ) ; 
    _current->add( lcioOutTypes  ) ; 
  }
}

StringParameters* XMLParser::getParameters( const std::string& sectionName ) const 
{
  return _map[ sectionName ] ;
}

void XMLParser::processconditions( TiXmlNode* current , const std::string& aCondition ) 
{
  std::string condition ;
  // put parentheses around compound expressions 
  if( aCondition.find('&') != std::string::npos  ||  aCondition.find('|') != std::string::npos ) condition = "(" + aCondition + ")" ;
  else condition = aCondition ;
  TiXmlNode* child = nullptr ;
  while( ( child = current->IterateChildren( "if" , child )  )  != nullptr  )processconditions( child , getAttribute( child, "condition") ) ;
  while( ( child = current->IterateChildren( "processor" , child )  )  != nullptr  ) 
  {
    if(  child->ToElement()->Attribute("condition") == nullptr ) child->ToElement()->SetAttribute("condition" ,  condition ) ;
    else 
    {
      std::string cond( child->ToElement()->Attribute("condition") ) ; 
      if( cond.size() > 0 && condition.size() )  cond += " && " ;
      cond += condition ;
      child->ToElement()->SetAttribute("condition" ,  cond ) ;
    }
    if( std::string( current->Value() ) != "execute" ) 
    {
      TiXmlNode* parent = current->Parent() ;
      std::unique_ptr<TiXmlNode> clone ( child->Clone() ) ;
      parent->InsertBeforeChild(  current , *clone ) ;  
    }
  }
  // remove the current <if/> node
  if( std::string( current->Value() ) != "execute" ) 
  {
    TiXmlNode* parent = current->Parent() ;
    parent->RemoveChild( current ) ;
  }    
}

void XMLParser::replacegroups(TiXmlNode* section) 
{
  TiXmlElement* root = _doc->RootElement()  ;
  if( root==nullptr )
  {
    std::cout<<red << "XMLParser::parse : no root tag <marlin>...</marlin> found ! " <<normal<< std::endl ;
    return ;
  }
  TiXmlNode* child = nullptr ;
  while( ( child = section->IterateChildren( child ) )  != nullptr  )
  {
    if( std::string( child->Value() )  == "group" ) 
    {
      // find group definition in root node 
      TiXmlNode* group = findElement( root, "group", "name" , getAttribute( child, "name") ) ;
      if( group != nullptr ) 
      {
        TiXmlNode* sub = nullptr ;
        while( ( sub = group->IterateChildren( "processor" , sub ) )  != nullptr  )
        {
          // insert <processor/> tag
          TiXmlElement item( "processor" );
          item.SetAttribute( "name",  getAttribute( sub, "name") ) ;
          section->InsertBeforeChild( child , item ) ;
        }
        section->RemoveChild( child ) ; 
      } 
      else std::cout<<red << " XMLParser::parse - group not found : " <<  child->ToElement()->Attribute("name")<<normal << std::endl ;
    } 
    else if( std::string( child->Value() )  == "if" )  
    {  // other element, e.g. <if></if>
      replacegroups( child ) ;
    }
  }
}

TiXmlNode* XMLParser::findElement( TiXmlNode* node , const std::string& type, const std::string& attribute, const std::string& value ) 
{
  TiXmlNode* child = nullptr ;
  bool elementFound  = false ;
  while( (child = node->IterateChildren( type , child ) )  != 0  )
  {
    if( std::string( *child->ToElement()->Attribute( attribute ) ) == value ) 
    { 
      elementFound = true ;
      break ;
    }
  }
  if( ! elementFound ) child = nullptr ;
  return child ;
}  
