#include "LogicalExpressions.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include "lcio.h"
 class ParseException : public lcio::Exception{
    
  protected:
    ParseException() {  /*no_op*/ ; } 
  public: 
    virtual ~ParseException() throw() { /*no_op*/; } 

    ParseException( std::string text ){
      message = "marlin::ParseException: " + text ;
    }
  }; 
  
  std::ostream& operator<< (  std::ostream& s,  Expression& e ) {
    
    if( e.Operation == Expression::AND ) s << " && "  ;
    else s << " || " ;
    if( e.isNot ) s << " ! " ;
    s << " [ " <<  e.Value << " ] "  << std::endl ;
    return s ;
  }

  LogicalExpressions::LogicalExpressions() {
    setValue("true",true);
    setValue("True",true);
    setValue("false",false);
    setValue("False",false);
  }
  
  
  void LogicalExpressions::addCondition( const std::string& name, const std::string& expression ) {
    _condMap[ name ] = expression ;

//     std::cout << " LogicalExpressions::addCondition( " << name << ", " << expression << " ) " << std::endl ;
  }
  
  void LogicalExpressions::clear() {

    for( ResultMap::iterator it = _resultMap.begin() ; it != _resultMap.end() ; it++){
      it->second = false ;
    }
    setValue("true",true);
    setValue("True",true);
    setValue("false",false);
    setValue("False",false);
//     std::cout << " LogicalExpressions::clear() "  << std::endl ;
  }
  
  bool LogicalExpressions::conditionIsTrue( const std::string& name ) {

    return expressionIsTrue( _condMap[ name ] ) ;
  }
  
  bool LogicalExpressions::expressionIsTrue( const std::string& expression ) {

    std::vector<Expression> tokens ;
    Tokenizer t( tokens ) ;
    
    std::for_each( expression.begin(),expression.end(), t ) ; 
    
    // atomic expression
    if( tokens.size() == 1 
	&& tokens[0].Value.find('&') == std::string::npos 
	&& tokens[0].Value.find('|') == std::string::npos ) { 
      
      if( tokens[0].isNot ) {

         return ! getValue( tokens[0].Value ) ;
      }
      else {

         return  getValue( tokens[0].Value ) ;
      }
    }	
	  
    bool returnVal = true ;

    for(   std::vector<Expression>::iterator it = tokens.begin() ; it != tokens.end() ; it++ ){

      bool tokenValue = expressionIsTrue( it->Value ) ;

      if( it->isNot ) 
	tokenValue = ! tokenValue ;

      if( it->Operation == Expression::AND ) 
	returnVal &= tokenValue ;
      else
	returnVal |= tokenValue ;
    }



    return returnVal ;
  }
  
  void LogicalExpressions::setValue( const std::string& key, bool val ) {

    _resultMap[ key ] = val ;
  }
  

  bool LogicalExpressions::getValue( const std::string& key) {
      ResultMap::iterator it = _resultMap.find( key );
      if (it == _resultMap.end() ) {
         std::ostringstream error; 
         error << "LogicalExpressions::getValue():  key \"" << key << "\" not found. Bad processor condition?\n";
 
	 //fg: debug:
	 for(  it = _resultMap.begin() ; it != _resultMap.end() ; ++it ){

	   std::cout << " key : " << it->first << " val: " << it->second << std::endl ;
	 }

	 throw ParseException( error.str() );
      }
      return it->second;
  }
