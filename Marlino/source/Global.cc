#include "Global.h"
#include "StringParameters.h"
#include "OutFileRoot.h"
#include "Geometry.h"
#include <vector>
#include <string>
StringParameters* Global::parameters = nullptr ;
OutFileRoot*   Global::out=nullptr;
Geometry* Global::geom=nullptr;
int Global::number=0;
int Global::totalevent=0;
