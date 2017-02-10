#ifndef LCTokenizer_h
#define LCTokenizer_h 1
#include<vector>
class LCTokenizer
{
  std::vector< std::string >& _tokens ;
  char _del ;
  char _last ;
  public:
    LCTokenizer( std::vector< std::string >& tokens, char del ) : _tokens(tokens) , _del(del), _last(del) {}
    void operator()(const char& c) 
    { 
      if( c != _del  ) 
      {
        if( _last == _del  ) _tokens.push_back("") ; 
	      _tokens.back() += c ;
	      result() ;
	    }
	    _last = c ;

    } 
    ~LCTokenizer(){}
    std::vector<std::string> & result()  
    { 
	    return _tokens ; 
    }
}; 
#endif
